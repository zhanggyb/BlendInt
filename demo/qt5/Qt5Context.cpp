/*
 * Qt5Context.cpp
 */

#include "Qt5Context.hpp"
#include <QtGui/QCursor>

Qt5Context::Qt5Context ()
: BI::Context (), m_cursor(Qt::ArrowCursor)
{
}

Qt5Context::~Qt5Context ()
{
}
