#pragma once

#include "umllib/private/lineHandler.h"

namespace qReal {

class CurveLine : public LineHandler
{
	Q_OBJECT
public:
	CurveLine(EdgeElement *edge);

	virtual void drawLine(QPainter *painter, bool drawSavedLine);
	virtual void drawPorts(QPainter *painter);
	virtual QPainterPath shape() const;

	virtual void adjust();

protected:
	void setBezierPoints();
	QPainterPath bezierCurve(QPolygonF const &line) const;

	virtual void handleEdgeMove(QPointF const &pos);

	virtual void drawPort(QPainter *painter, int portNumber);
	void drawControlLines(QPainter *painter);
};

}
