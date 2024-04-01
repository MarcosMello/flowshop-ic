#include <utility>

#include "ComparisonTable.h"

using namespace std;

string generateHeader(const string &caption, const string &label, const bool hasReferenceValue) {
    if (hasReferenceValue) {
        return R"(\begin{table}[H]
        \renewcommand{\arraystretch}{1.2}
        \centering
        \caption{)" + caption + R"(} \label{)" + label + R"(}
        \resizebox{\textwidth}{!}{%
            \begin{tabular}{l *{12}{c}}
                \toprule
                \multirow[c]{2}{*}{\textbf{}} && \multirow[c]{2}{*}{\textbf{Instância}} && \multirow[c]{2}{*}{\shortstack[c]{\textbf{Solução}\\ \textbf{Ótima}}} && \multicolumn{3}{c}{\textbf{CPLEX}} && \multicolumn{3}{c}{\textbf{AG}} \\
                \cline{7-9} \cline{11-13}
                && && && \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} && \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} \\
                \midrule)";
    }

    return R"(\begin{table}[ht!]
            \renewcommand{\arraystretch}{1.2}
            \centering
            \caption{)" + caption + R"(} \label{)" + label + R"(}
            \resizebox{\textwidth}{!}{%
                \begin{tabular}{l *{9}{c}}
                    \toprule
                    \multirow[c]{2}{*}{\textbf{}} & \multirow[c]{2}{*}{\textbf{Instância}} && \multicolumn{3}{c}{\textbf{CPLEX}} && \multicolumn{3}{c}{\textbf{AG}} \\
                    \cline{4-6} \cline{8-10} && & \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} && \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} \\
                    \midrule)";
}

Line::Line(string filename, string instance, const IloNum cplexSolution, const chrono::duration<double> cplexTimeElapsed,
    const size_t gaSolution, const chrono::duration<double> gaTimeElapsed) : filename(std::move(filename)),
    instance(std::move(instance)), cplexSolution(cplexSolution), cplexTimeElapsed(cplexTimeElapsed), gaSolution(gaSolution),
    gaTimeElapsed(gaTimeElapsed) {}

Line::Line(string filename, string instance, const int referenceValue, const IloNum cplexSolution,
    const chrono::duration<double> cplexTimeElapsed, const size_t gaSolution,
    const chrono::duration<double> gaTimeElapsed) : filename(std::move(filename)),
    instance(std::move(instance)), hasReferenceValue(true), referenceValue(referenceValue), cplexSolution(cplexSolution),
    cplexTimeElapsed(cplexTimeElapsed), gaSolution(gaSolution), gaTimeElapsed(gaTimeElapsed) {}

ostream &operator<<(ostream &stream, const Line& line) {
    if (line.hasReferenceValue) {
        stream << R"(\textbf{)" << line.filename << "} &&" << line.instance << "&&" << line.referenceValue << "&&"
        << line.cplexSolution << "&" << line.cplexTimeElapsed.count() << "&" << 0
        << "&&" << line.gaSolution << "&" << line.gaTimeElapsed.count() << "&"
        << 100 * (static_cast<double>(line.gaSolution - line.referenceValue)/line.referenceValue) << R"(\\ \hline)";

        return stream;
    }

    const double referenceValue = min(
        line.cplexSolution,
        static_cast<double>(line.gaSolution)
    );

    const double method = max(
        line.cplexSolution,
        static_cast<double>(line.gaSolution)
    );

    double differenceCPLEX, differenceGA;

    if (line.cplexSolution < static_cast<double>(line.gaSolution)) {
        differenceGA = 0;
        differenceCPLEX = 100 * ((method - referenceValue) / referenceValue);
    } else {
        differenceGA = 100 * ((method - referenceValue) / referenceValue);;
        differenceCPLEX = 0;
    }

    stream << R"(\textbf{)" << line.filename << "} &" << line.instance << "&&" << line.cplexSolution << "&"
        << line.cplexTimeElapsed.count() << "&" << differenceCPLEX << "&&" << line.gaSolution << "&"
        << line.gaTimeElapsed.count() << "&"
        << differenceGA << R"(\\ \hline)";

    return stream;
}

Table::Table(const string &caption, const string &label, const bool hasReferenceValue) :
    header(generateHeader(caption, label, hasReferenceValue)),
    footer(R"(\bottomrule
                \end{tabular}
            }
            \caption*{Fonte: Produção do próprio autor.}
        \end{table})"),
    hasReferenceValue(hasReferenceValue){}

void Table::addLine(const Line &line) {
    return this->lines.push_back(line);
}

ostream &operator<<(ostream &stream, const Table& table) {
    stream << table.header << endl;

    for (const auto& line: table.lines) {
        stream << line << endl;
    }

    stream << table.footer << endl;
    return stream;
}