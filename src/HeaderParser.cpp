#include "HeaderParser.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

const QRegularExpression HeaderParser::classRegex(
    R"((?:template\s*<[^>]*>\s*)?class\s+([\w_]+)\s*(?::\s*(?:public|protected|private)?\s*([\w_:]+)(?:\s*,\s*(?:public|protected|private)?\s*([\w_:]+))*)?\s*\{)");

QMap<QString, ClassInfo> HeaderParser::parseHeader(const QString& filePath, bool& ok) {
    ok = false;
    QMap<QString, ClassInfo> classes;
    QFile                    file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << filePath;
        return classes;
    }

    QTextStream in(&file);
    QString     content = in.readAll();
    file.close();

    QRegularExpressionMatchIterator i = classRegex.globalMatch(content);
    while (i.hasNext()) {
        QRegularExpressionMatch match     = i.next();
        QString                 className = match.captured(1);
        if (!className.isEmpty()) {
            if (!classes.contains(className)) {
                classes[className] = ClassInfo{className};
            }

            QString base1 = match.captured(2);
            QString base2 = match.captured(3);

            if (!base1.isEmpty()) {
                classes[className].baseClasses.append(base1.split("::").last());
            }

            if (!base2.isEmpty()) {
                classes[className].baseClasses.append(base2.split("::").last());
            }
        }
    }

    return ok = !classes.isEmpty(), classes;
}