#ifndef HEADERPARSER_H
#define HEADERPARSER_H

#include <QList>
#include <QMap>
#include <QPair>
#include <QPointF>
#include <QRegularExpression>
#include <QString>

struct ClassInfo {
    QString     name;
    QStringList baseClasses;
    QPointF     position        = QPointF(0, 0);
    int         level           = 0;
    int         horizontalIndex = 0;
    bool        positioned      = false;
};

class HeaderParser {
   public:
    static QMap<QString, ClassInfo> parseHeader(const QString& filePath, bool& ok);

   private:
    static const QRegularExpression classRegex;
};

#endif  // HEADERPARSER_H