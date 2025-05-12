#ifndef DIAGRAMMANAGER_H
#define DIAGRAMMANAGER_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariantList>

#include "HeaderParser.h"

class DiagramManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList classesModel READ classesModel NOTIFY classesModelChanged)
    Q_PROPERTY(QVariantList connectionsModel READ connectionsModel NOTIFY connectionsModelChanged)

   public:
    explicit DiagramManager(QObject* parent = nullptr);

    QVariantList classesModel() const;
    QVariantList connectionsModel() const;

    Q_INVOKABLE void openFile();

   signals:
    void classesModelChanged();
    void connectionsModelChanged();
    void errorOccurred(const QString& message);

   private:
    void parseHeaderFile(const QString& filePath);
    void calculateLayout();
    void updateModels();
    void assignLevels(const QString& className, int level);
    void calculatePositions();

    QMap<QString, ClassInfo> m_classes;
    QVariantList             m_classesModel;
    QVariantList             m_connectionsModel;

    const qreal BOX_WIDTH  = 150.0;
    const qreal BOX_HEIGHT = 50.0;
    const qreal H_SPACING  = 50.0;
    const qreal V_SPACING  = 80.0;
};

#endif  // DIAGRAMMANAGER_H