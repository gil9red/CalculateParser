#ifndef UCALCULATEPARSER_H
#define UCALCULATEPARSER_H

#include <QtCore>

#include <QTextDocument>
#include <QTextCursor>

const QString regExpNumber = "[0-9]+([,.]?[0-9]+)?";
const QString regExpFunction = "[\+\-\/\*\%\^]";
const QString regExpBrackets = "[\(\)]";

const QString regExpUOOFunction = "%1\\(" + QString("[-+]?") + regExpNumber + "\\)"; // TODO
const QString regExpSin = regExpUOOFunction.arg("sin");
const QString regExpCos = regExpUOOFunction.arg("cos");
const QString regExpTan = regExpUOOFunction.arg("tan");
const QString regExpASin = regExpUOOFunction.arg("asin");
const QString regExpACos = regExpUOOFunction.arg("acos");
const QString regExpATan = regExpUOOFunction.arg("atan");
const QString regExpSqrt = regExpUOOFunction.arg("sqrt");
const QString regExpLn = regExpUOOFunction.arg("ln");
const QString regExpExp = regExpUOOFunction.arg("exp");
const QString regExpFloor = regExpUOOFunction.arg("floor");
const QString regExpCeil = regExpUOOFunction.arg("ceil");
const QString regExpAbs = regExpUOOFunction.arg("abs");

const QString regExpMathFunction = regExpASin
        + "|" + regExpACos
        + "|" + regExpATan
        + "|" + regExpSin
        + "|" + regExpCos
        + "|" + regExpTan
        + "|" + regExpSqrt
        + "|" + regExpLn
        + "|" + regExpExp
        + "|" + regExpFloor
        + "|" + regExpCeil
        + "|" + regExpAbs;

class UCalculateParser
{
public:
    UCalculateParser();
    UCalculateParser(const QString expressions);
    qreal calculate(bool debug = false);
    qreal calculate(const QString expressions, bool debug = false);

    void setExpressions(const QString expressions);
    QString expressions();

private:
    QString d_expressions;
    bool validExpressions;
};

#endif // UCALCULATEPARSER_H
