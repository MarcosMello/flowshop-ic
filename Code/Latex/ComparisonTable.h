#ifndef COMPARISONTABLE_H
#define COMPARISONTABLE_H

#include "../cplexModel.h"

class Line {
public:
    Line(string filename, IloNum cplexSolution, chrono::duration<double> cplexTimeElapsed,
        size_t gaSolution, chrono::duration<double> gaTimeElapsed);

    friend ostream &operator<<(ostream &stream, const Line& line);

private:
    string filename;
    int instance = 0;

    IloNum cplexSolution;
    chrono::duration<double> cplexTimeElapsed;

    size_t gaSolution;
    chrono::duration<double> gaTimeElapsed;

    double difference = 0;
};

class Table {
public:
    Table(const string &caption, const string &label);

    friend ostream &operator<<(ostream &stream, const Table& table);

private:
    string header;
    string footer;

    vector<Line> lines;
};

#endif //COMPARISONTABLE_H
