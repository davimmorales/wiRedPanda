#include "graphicelement.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>

GraphicElement::GraphicElement(QPixmap pixmap, QGraphicsItem *parent) : QGraphicsItem(parent), pixmapItem(new QGraphicsPixmapItem(pixmap, ( QGraphicsItem * ) this)) {
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  m_topPosition = 0;
  m_bottomPosition = 64;
  m_maxInputSz = 8;
  m_maxOutputSz = 1;
  m_outputsOnTop = true;
}

GraphicElement::~GraphicElement() {

}
QVector<QNEPort *> GraphicElement::outputs() const {
  return m_outputs;
}

void GraphicElement::setOutputs(const QVector<QNEPort *> & outputs) {
  m_outputs = outputs;
}

QVector<QNEPort *> GraphicElement::inputs() const {
  return m_inputs;
}

void GraphicElement::setInputs(const QVector<QNEPort *> & inputs) {
  m_inputs = inputs;
}


QRectF GraphicElement::boundingRect() const {
  return (pixmapItem->boundingRect());
}

void GraphicElement::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  if(isSelected()) {
    painter->setBrush(Qt::darkGray);
    painter->setPen(QPen(Qt::black));
    painter->drawRoundedRect(boundingRect(),16,16);
  }
}

QNEPort *GraphicElement::addPort(bool isOutput) {
  if(isOutput && m_outputs.size() >= m_maxOutputSz) {
    return 0;
  } else if(!isOutput && m_inputs.size() >= m_maxInputSz) {
    return 0;
  }
  QNEPort *port = new QNEPort(this);
  port->setIsOutput(isOutput);
  port->setGraphicElement(this);
  int inputPos;
  int outputPos;
  if(m_outputsOnTop) {
    inputPos = m_bottomPosition;
    outputPos = m_topPosition;
  } else {
    inputPos = m_topPosition;
    outputPos = m_bottomPosition;
  }
  if(isOutput) {
    m_outputs.push_back(port);
    int step = qMax(32/m_outputs.size(), 6);
    int x = 32 - m_outputs.size()*step + step;
    foreach (QNEPort * port, m_outputs) {
      port->setPos(x,outputPos);
      x+= step * 2;
    }
  } else {
    m_inputs.push_back(port);
    int step = qMax(32/m_inputs.size(),6);
    int x = 32 - m_inputs.size()*step + step;
    foreach (QNEPort * port, m_inputs) {
      port->setPos(x,inputPos);
      x+= step * 2;
    }
  }
  port->show();
  return port;
}

void GraphicElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * e) {
  if(e->button() == Qt::LeftButton) {
    addPort(QNEPort::Output);
  } else {
    addPort(QNEPort::Input);
  }
}

QVariant GraphicElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value) {
  if(change == ItemScenePositionHasChanged ||  change == ItemRotationHasChanged ||  change == ItemTransformHasChanged) {
    foreach (QNEPort * port, m_outputs) {
      port->updateConnections();
    }
    foreach (QNEPort * port, m_inputs) {
      port->updateConnections();
    }
  }
  return value;
}

int GraphicElement::maxOutputSz() const {
  return m_maxOutputSz;
}

void GraphicElement::setMaxOutputSz(int maxOutputSz) {
  m_maxOutputSz = maxOutputSz;
}

int GraphicElement::maxInputSz() const {
  return m_maxInputSz;
}

void GraphicElement::setMaxInputSz(int maxInputSz) {
  m_maxInputSz = maxInputSz;
}

int GraphicElement::bottomPosition() const {
  return m_bottomPosition;
}

void GraphicElement::setBottomPosition(int bottomPosition) {
  m_bottomPosition = bottomPosition;
}

int GraphicElement::topPosition() const {
  return m_topPosition;
}

void GraphicElement::setTopPosition(int topPosition) {
  m_topPosition = topPosition;
}

bool GraphicElement::outputsOnTop() const {
  return m_outputsOnTop;
}

void GraphicElement::setOutputsOnTop(bool outputsOnTop) {
  m_outputsOnTop = outputsOnTop;
}