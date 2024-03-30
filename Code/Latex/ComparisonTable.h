#ifndef COMPARISONTABLE_H
#define COMPARISONTABLE_H

#include "../cplexModel.h"

class Line {
public:
    Line(string filename, string instance, IloNum cplexSolution, chrono::duration<double> cplexTimeElapsed,
        size_t gaSolution, chrono::duration<double> gaTimeElapsed);
    Line(string filename, string instance, int referenceValue, IloNum cplexSolution, chrono::duration<double> cplexTimeElapsed,
        size_t gaSolution, chrono::duration<double> gaTimeElapsed);

    friend ostream &operator<<(ostream &stream, const Line& line);

private:
    string filename;
    string instance;

    bool hasReferenceValue = false;
    int referenceValue = 0;

    IloNum cplexSolution;
    chrono::duration<double> cplexTimeElapsed;

    size_t gaSolution;
    chrono::duration<double> gaTimeElapsed;

    double difference = 0;
};

class Table {
public:
    Table(const string &caption, const string &label, bool hasReferenceValue);

    void addLine(const Line &line);

    friend ostream &operator<<(ostream &stream, const Table& table);

private:
    string header;
    string footer;

    bool hasReferenceValue;

    vector<Line> lines;
};

#endif //COMPARISONTABLE_H
