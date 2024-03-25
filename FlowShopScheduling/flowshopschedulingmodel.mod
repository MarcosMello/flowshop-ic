/*********************************************
 * OPL 22.1.1.0 Model
 * Author: marcos
 * Creation Date: Dec 18, 2023 at 3:52:44 PM
 *********************************************/

int jobs = ...;
int machines = ...;

range jobsRange = 1 .. jobs;
range machinesRange = 1 .. machines;

int processingTime[jobsRange][machinesRange] = ...;
int deadlines[jobsRange] = ...;

dvar boolean position[jobsRange][jobsRange];

dvar int+ earliness[jobsRange];
dvar int+ lateness[jobsRange];

dvar int+ conclusion[jobsRange];

dvar int+ waitingTime[jobsRange][1 .. (machines - 1)];

dvar int+ inactivity[1 .. (jobs - 1)][machinesRange];
dvar int+ firstInactivity;

minimize
  sum(job in jobsRange)
    (earliness[job] + lateness[job]);
    
subject to{
  	conclusion[1] == firstInactivity + 
  		(sum (machine in machinesRange) sum (job in jobsRange)
  		  (position[job][1] * processingTime[job][machine])) +
	  	(sum (machine in 1 .. (machines - 1)) waitingTime[1][machine]);
  
  	forall (job in 2 .. jobs){
  		conclusion[job] == conclusion[job - 1] + 
  			inactivity[job - 1][machines] +
  			(sum (jobPosition in jobsRange) 
  				(position[jobPosition][job] * processingTime[jobPosition][machines]));
  	}
  	
  	forall (job in 1 .. jobs - 1){
  		forall (machine in 1 .. machines - 1){
  			inactivity[job][machine] + 
			(sum (jobPosition in jobsRange) 
				(position[jobPosition][job + 1] * 
				processingTime[jobPosition][machine])
			) + waitingTime[job + 1][machine] == 
				waitingTime[job][machine] + 
				(sum (jobPosition in jobsRange)
					(position[jobPosition][job] *
					processingTime[jobPosition][machine + 1])
				) +
				inactivity[job][machine + 1];
  		}
  	}
  
	forall (jobPosition in jobsRange){
	  sum (jobInPosition in jobsRange) 
	  	position[jobPosition][jobInPosition] == 1;
	  	
	  sum (jobInPosition in jobsRange)
	  	position[jobInPosition][jobPosition] == 1;
 	}
 	
 	forall (job in jobsRange){
 		earliness[job] >= conclusion[job] - 
 			(sum (jobPosition in jobsRange) 
 				(position[jobPosition][job] * deadlines[jobPosition]));
		lateness[job] >= (sum (jobPosition in jobsRange) 
			(position[jobPosition][job] * deadlines[jobPosition])) 
			- conclusion[job];
 	}
}

main {
  	thisOplModel.generate();
  	thisOplModel.getObjective();
	cplex.solve();
	writeln(cplex.getObjValue());
	
	write("Schedule: ( ")
  	for(var i = 1; i <= thisOplModel.jobs; i++){
  	  for(var j = 1; j <= thisOplModel.jobs; j++){
  	    if(thisOplModel.position[j][i] == 1) write(j + " ");
  	  }
  	}
  	write(")");
}