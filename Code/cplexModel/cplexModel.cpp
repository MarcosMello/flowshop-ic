#include "../cplexModel.h"

CplexSolver::CplexSolver(const InputData &instanceData) :
    jobs(instanceData.jobs),
    machines(instanceData.machines),
    processingTime(instanceData.processingTime),
    deadlines(instanceData.deadlines) {
    IloEnv env;

    try {
        const auto timerStart = chrono::system_clock::now();

        IloArray<IloIntVarArray> position(env, jobs);

        for (int i = 0; i < jobs; i++) {
            position[i] = IloIntVarArray(env, jobs, 0, IloIntMax);
        }

        this->variantPosition = position;

        const IloCplex cplexSolver = modelSolver(env);

        const auto timerEnd = chrono::system_clock::now();
        const chrono::duration<double> timeSpent = timerEnd - timerStart;

        this->cplexSolution = cplexSolver.getObjValue();
        this->cplexGap = cplexSolver.getMIPRelativeGap();
        this->cplexTimeElapsed = timeSpent;

        for (int i = 0; i < jobs; i++) {
            for(int j = 0; j < jobs; j++) {
                if(cplexSolver.getIntValue(position[j][i])) {
                    this->individual.emplace_back(j);
                }
            }
        }
    }
    catch ([[maybe_unused]] IloException &e) {
        cerr << "Concert exception caught" << endl;
        throw;
    } catch (...) {
        cerr << "Unknown exception caught" << endl;
        throw;
    }

    env.end();
}

CplexSolver::CplexSolver(const InputData& instanceData, const vector<int>& individual) :
    CplexSolver(individual,
                instanceData.jobs,
                instanceData.machines,
                instanceData.processingTime,
                instanceData.deadlines
    ) {}

CplexSolver::CplexSolver(const vector<int>& individual,
                         const int jobs,
                         const int machines,
                         const vector<vector<int>>& processingTime,
                         const vector<int>& deadlines) :
    individual(individual),
    jobs(jobs),
    machines(machines),
    processingTime(processingTime),
    deadlines(deadlines) {
    IloEnv env;

    try {
        const auto timerStart = chrono::system_clock::now();

        vector position(this->jobs, vector(this->jobs, 0));

        for (size_t i = 0; i < position.size(); i++) {
            position[this->individual[i]][i] = 1;
        }

        this->variantPosition = position;

        const IloCplex cplexSolver = modelSolver(env);

        const auto timerEnd = chrono::system_clock::now();
        const chrono::duration<double> timeSpent = timerEnd - timerStart;

        this->cplexSolution = cplexSolver.getObjValue();
        this->cplexGap = cplexSolver.getMIPRelativeGap();
        this->cplexTimeElapsed = timeSpent;
    }
    catch ([[maybe_unused]] IloException &e) {
        cerr << "Concert exception caught" << endl;
        throw;
    } catch (...) {
        cerr << "Unknown exception caught" << endl;
        throw;
    }

    env.end();
}

IloCplex CplexSolver::modelSolver(IloEnv env) {
    return visit([this, env](auto&& position){
        IloModel model(env);

        IloIntVarArray earliness(env, jobs, 0, IloIntMax);
        IloIntVarArray tardiness(env, jobs, 0, IloIntMax);

        IloIntVarArray conclusion(env, jobs, 0, IloIntMax);

        IloArray<IloIntVarArray> waitingTime(env, jobs);
        IloArray<IloIntVarArray> inactivity(env, jobs - 1);

        IloIntVar firstInactivity(env, 0, IloIntMax);

        for (int i = 0; i < jobs; i++) {
            waitingTime[i] = IloIntVarArray(env, machines, 0, IloIntMax);

            if (i < jobs - 1) {
                inactivity[i] = IloIntVarArray(env, machines, 0, IloIntMax);
            }
        }

        IloExpr objective(env);
        for (int i = 0; i < jobs; i++) {
            objective += (earliness[i] + tardiness[i]);
        }

        model.add(IloMinimize(env, objective));

        IloConstraintArray constraints(env);

        IloExpr firstJobConclusionTime(env);
        for (int k = 0; k < machines; k++){
            for (int i = 0; i < jobs; i++){
                firstJobConclusionTime += position[i][0] * processingTime[i][k];
            }

            firstJobConclusionTime += waitingTime[0][k];
        }

        constraints.add(conclusion[0] == firstInactivity + firstJobConclusionTime);

        IloExprArray jobConclusionTime(env, jobs);

        for (int i = 1; i < jobs; i++){
            jobConclusionTime[i] = IloExpr(env);

            for (int j = 0; j < jobs; j++){
                jobConclusionTime[i] += position[j][i] * processingTime[j][machines - 1];
            }

            constraints.add(conclusion[i] == conclusion[i - 1] + inactivity[i - 1][machines - 1] + jobConclusionTime[i]);
        }

        IloArray<IloExprArray> processingTimeOnActualMachine(env, jobs);
        IloArray<IloExprArray> processingTimeOnNextMachine(env, jobs);

        for (int i = 0; i < jobs - 1; i++){
            processingTimeOnActualMachine[i] = IloExprArray(env, machines);
            processingTimeOnNextMachine[i] = IloExprArray(env, machines);

            for (int k = 0; k < machines - 1; k++){
                processingTimeOnActualMachine[i][k] = IloExpr(env);
                processingTimeOnNextMachine[i][k] = IloExpr(env);

                for (int j = 0; j < jobs; j++) {
                    processingTimeOnActualMachine[i][k] += position[j][i + 1] * processingTime[j][k];
                    processingTimeOnNextMachine[i][k] += position[j][i] * processingTime[j][k + 1];
                }

                constraints.add(inactivity[i][k] + processingTimeOnActualMachine[i][k] + waitingTime[i + 1][k] ==
                                inactivity[i][k + 1] + processingTimeOnNextMachine[i][k] + waitingTime[i][k]);
            }
        }

        IloExprArray positionRows(env, jobs);
        IloExprArray positionColumns(env, jobs);

        IloExprArray conclusionByJob(env, jobs);

        for (int i = 0; i < jobs; i++){
            positionRows[i] = IloExpr(env);
            positionColumns[i] = IloExpr(env);

            conclusionByJob[i] = IloExpr(env);

            for (int j = 0; j < jobs; j++){
                positionRows[i] += position[i][j];
                positionColumns[i] += position[j][i];

                conclusionByJob[i] += position[j][i] * deadlines[j];
            }

            constraints.add(positionRows[i] == 1);
            constraints.add(positionColumns[i] == 1);

            constraints.add(earliness[i] >= conclusion[i] - conclusionByJob[i]);
            constraints.add(tardiness[i] >= conclusionByJob[i] - conclusion[i]);
        }

        model.add(constraints);

        IloCplex cplexSolver(model);
        cplexSolver.setParam(IloCplex::Param::TimeLimit, timeLimitInSeconds);
        cplexSolver.setParam(IloCplex::Param::Threads, threadLimit);

        if(!isCplexVerboseActive) cplexSolver.setOut(env.getNullStream());

        if (!cplexSolver.solve()){
            env.error() << "Failed to optimize LP." << endl;
            throw;
        }

        return cplexSolver;
    }, variantPosition);
}

double CplexSolver::getCplexSolution() const {
    return this->cplexSolution;
}

chrono::duration<double> CplexSolver::getCplexTimeElapsed() const {
    return this->cplexTimeElapsed;
}

void CplexSolver::print() const {
    cout << "Objective Value: " << this->cplexSolution << endl;

    cout << "Gap: " << this->cplexGap << endl;

    cout << "Job Order: ";
    for(int i = 0; i < jobs; i++) {
        cout << (individual[i] + 1) << " \n" [i == jobs];
    }
    cout << endl;

    cout << "Time spent: " << cplexTimeElapsed.count() << endl;
}
