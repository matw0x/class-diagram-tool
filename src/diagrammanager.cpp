#include "diagrammanager.h"

#include <QDebug>
#include <QFileDialog>
#include <QQueue>
#include <QSet>
#include <QStandardPaths>
#include <QVariantMap>
#include <algorithm>
#include <cmath>

#include "HeaderParser.h"

DiagramManager::DiagramManager(QObject* parent) : QObject(parent) {}

QVariantList DiagramManager::classesModel() const { return m_classesModel; }

QVariantList DiagramManager::connectionsModel() const { return m_connectionsModel; }

void DiagramManager::openFile() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Open Header File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Header Files (*.h *.hpp)"));

    if (!filePath.isEmpty()) {
        parseHeaderFile(filePath);
    }
}

void DiagramManager::parseHeaderFile(const QString& filePath) {
    m_classes.clear();
    m_classesModel.clear();
    m_connectionsModel.clear();

    bool parseOk;
    m_classes = HeaderParser::parseHeader(filePath, parseOk);

    if (parseOk) {
        calculateLayout();
        updateModels();
    } else {
        emit errorOccurred(tr("Could not parse header file or no classes found."));
        emit classesModelChanged();
        emit connectionsModelChanged();
    }
}

void DiagramManager::calculateLayout() {
    if (m_classes.isEmpty()) return;

    for (auto it = m_classes.begin(); it != m_classes.end(); ++it) {
        it->level           = -1;
        it->positioned      = false;
        it->horizontalIndex = 0;
    }

    QList<QString> roots;
    for (const auto& info : m_classes) {
        bool isRoot = true;
        if (!info.baseClasses.isEmpty()) {
            for (const QString& base : info.baseClasses) {
                if (m_classes.contains(base)) {
                    isRoot = false;
                    break;
                }
            }
        }

        if (isRoot) {
            roots.append(info.name);
        }
    }

    if (roots.isEmpty() && !m_classes.isEmpty()) {
        roots.append(m_classes.firstKey());
    }

    QQueue<QString> queue;
    QSet<QString>   visited;
    int             currentLevel = 0;

    for (const QString& root : roots) {
        if (m_classes.contains(root)) {
            m_classes[root].level = currentLevel;
            queue.enqueue(root);
            visited.insert(root);
        }
    }

    while (!queue.isEmpty()) {
        int levelSize = queue.size();
        currentLevel++;
        for (int i = 0; i < levelSize; ++i) {
            QString currentClass = queue.dequeue();

            for (auto it = m_classes.begin(); it != m_classes.end(); ++it) {
                if (it->baseClasses.contains(currentClass) && !visited.contains(it.key())) {
                    it->level = std::max(it->level, currentLevel);
                    queue.enqueue(it.key());
                    visited.insert(it.key());
                }
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto it = m_classes.begin(); it != m_classes.end(); ++it) {
            if (it->level == -1) {
                int  maxBaseLevel      = -1;
                bool allBasesHaveLevel = !it->baseClasses.isEmpty();
                for (const QString& baseName : it->baseClasses) {
                    if (m_classes.contains(baseName) && m_classes[baseName].level != -1) {
                        maxBaseLevel = std::max(maxBaseLevel, m_classes[baseName].level);
                    } else if (m_classes.contains(baseName)) {
                        allBasesHaveLevel = false;
                        break;
                    }
                }
                if (allBasesHaveLevel && maxBaseLevel != -1) {
                    it->level = maxBaseLevel + 1;
                    changed   = true;
                } else if (it->baseClasses.isEmpty()) {
                    it->level = 0;
                    changed   = true;
                }
            }
        }
    }

    for (auto it = m_classes.begin(); it != m_classes.end(); ++it) {
        if (it->level == -1) it->level = 0;
    }

    QMap<int, QList<QString>> levels;
    int                       maxLevel = 0;
    for (const auto& info : m_classes) {
        levels[info.level].append(info.name);
        maxLevel = std::max(maxLevel, info.level);
    }

    for (int level = 0; level <= maxLevel; ++level) {
        qreal currentX = 0;
        int   index    = 0;
        for (const QString& className : levels[level]) {
            if (m_classes.contains(className)) {
                m_classes[className].position        = QPointF(currentX, level * (BOX_HEIGHT + V_SPACING));
                m_classes[className].positioned      = true;
                m_classes[className].horizontalIndex = index++;
                currentX += BOX_WIDTH + H_SPACING;
            }
        }
    }

    qreal maxTotalWidth = 0;
    for (int level = 0; level <= maxLevel; ++level) {
        if (!levels[level].isEmpty()) {
            qreal levelWidth = levels[level].size() * BOX_WIDTH + (levels[level].size() - 1) * H_SPACING;
            maxTotalWidth    = std::max(maxTotalWidth, levelWidth);
        }
    }

    for (int level = 0; level <= maxLevel; ++level) {
        if (!levels[level].isEmpty()) {
            qreal levelWidth = levels[level].size() * BOX_WIDTH + (levels[level].size() - 1) * H_SPACING;
            qreal offset     = (maxTotalWidth - levelWidth) / 2.0;
            if (offset < 0) offset = 0;

            for (const QString& className : levels[level]) {
                if (m_classes.contains(className)) {
                    m_classes[className].position.setX(m_classes[className].position.x() + offset);
                }
            }
        }
    }
}

void DiagramManager::updateModels() {
    m_classesModel.clear();
    m_connectionsModel.clear();

    for (const auto& info : m_classes) {
        if (info.positioned) {
            QVariantMap classMap;
            classMap["name"]   = info.name;
            classMap["x"]      = info.position.x();
            classMap["y"]      = info.position.y();
            classMap["width"]  = BOX_WIDTH;
            classMap["height"] = BOX_HEIGHT;
            m_classesModel.append(classMap);

            for (const QString& baseName : info.baseClasses) {
                if (m_classes.contains(baseName) && m_classes[baseName].positioned) {
                    const auto& baseInfo = m_classes[baseName];
                    QVariantMap connectionMap;

                    connectionMap["x1"] = baseInfo.position.x() + BOX_WIDTH / 2.0;
                    connectionMap["y1"] = baseInfo.position.y() + BOX_HEIGHT;
                    connectionMap["x2"] = info.position.x() + BOX_WIDTH / 2.0;
                    connectionMap["y2"] = info.position.y();
                    m_connectionsModel.append(connectionMap);
                }
            }
        }
    }

    emit classesModelChanged();
    emit connectionsModelChanged();
}