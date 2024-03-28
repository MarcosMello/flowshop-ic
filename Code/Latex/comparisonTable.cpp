#include <utility>

#include "ComparisonTable.h"

using namespace std;

Line::Line(string filename, const IloNum cplexSolution, const chrono::duration<double> cplexTimeElapsed,
    const size_t gaSolution, const chrono::duration<double> gaTimeElapsed) : filename(std::move(filename)),
    cplexSolution(cplexSolution), cplexTimeElapsed(cplexTimeElapsed), gaSolution(gaSolution),
    gaTimeElapsed(gaTimeElapsed) {}

ostream &operator<<(ostream &stream, const Line& line) {
    stream << R"(\textbf{)" << line.filename << "} &" << line.instance << "&&" << line.cplexSolution << "&"
        << line.cplexTimeElapsed.count() << "&" << 0 << "&&" << line.gaSolution << "&"
        << line.gaTimeElapsed.count() << "&" << 0 << R"(\\ \hline)";

    return stream;
}

Table::Table() :
    header(R"(\begin{table}[ht!]
            \renewcommand{\arraystretch}{1.2}
            \centering
            \caption{Resultados obtidos para as instâncias com data de entrega comum} \label{Tab-Resultados-Comum}
            \resizebox{\textwidth}{!}{%
                \begin{tabular}{l *{9}{c}}
                    \toprule
                    \multirow[c]{2}{*}{\textbf{}} & \multirow[c]{2}{*}{\textbf{Instância}} && \multicolumn{3}{c}{\textbf{CPLEX}} && \multicolumn{3}{c}{\textbf{AG}} \\
                    \cline{4-6} \cline{8-10} && & \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} && \textbf{Solução} & \textbf{Tempo(seg)} & \textbf{\%Diferença} \\
                    \midrule)"),
    footer(R"(\bottomrule
                \end{tabular}
            }
            \caption*{Fonte: Produção do próprio autor.}
        \end{table})"){}

ostream &operator<<(ostream &stream, const Table& table) {
    stream << table.header << endl;

    for (const auto& line: table.lines) {
        cout << line << endl;
    }

    stream << table.footer << endl;
    return stream;
}
