#include "TriangleNode.h"
#include "LevelView.h"

#include <QtCore>
#include <QtGui>
#include <QDebug>

using namespace Zk::LevelEditor;

TriangleNode::TriangleNode(LevelView * lv, QGraphicsItem * parent)
	: QGraphicsEllipseItem(parent)
{
	parentView = lv;
	
	setPen(QPen(Qt::black));
	setBrush(QBrush(Qt::white));
	setRect(QRect(-8, -8, 16, 16));
	
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setZValue(1.0);
}

TriangleNode::~TriangleNode()
{
	
}

void TriangleNode::attachTriangle(MeshTriangle * mt)
{
	linkedTriangles << mt;
}

void TriangleNode::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsEllipseItem::mousePressEvent(event);
	emit clicked(this);
}

void TriangleNode::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	qDebug() << "DUPA";
	QAction * removeAction = menu.addAction("&Remove");
	menu.exec(event->screenPos());
}

QVariant TriangleNode::itemChange(
	GraphicsItemChange change,
	const QVariant & value
)
{
	if (change == ItemPositionChange)
		emit moved(this, value.toPointF());
	
	return QGraphicsEllipseItem::itemChange(change, value);
}
