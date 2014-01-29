#include "UCalculateParser.h"

/// FUNCTION
bool isValidExpressions(const QString expressions);

bool isFunction(const QString f);
bool isFunction(const QChar f);
bool isOperand(const QString o);
bool isOperand(const QChar o);
bool isBracket(const QString s);
bool isBracket(const QChar s);

QStringList parse(QString expressions);
void popFunction(QStack <QString> *o, QStack <QString> *f);
bool canPop(QString op1, QStack <QString> *f);
int getPriority(QString op);
QString removeSpace(QString s);

/// PUBLIC
UCalculateParser::UCalculateParser()
{

}
UCalculateParser::UCalculateParser(const QString expressions)
{
    setExpressions(expressions);
}
qreal UCalculateParser::calculate(bool debug/* = false*/)
{
    return calculate(d_expressions, debug);
}
qreal UCalculateParser::calculate(const QString expressions, bool debug /*= false*/)
{
    validExpressions = isValidExpressions(expressions);
    if(!validExpressions)
    {
        qWarning() << "Ошибка: неправильное выражение";
        return 0.;
    }

    QString tExp = expressions;

    tExp = removeSpace(tExp);
    tExp = QString("(%1)").arg(tExp);

//Алгоритм Бауэра и Замельзона

    QStack <QString> operands;
    QStack <QString> functions;

    QString prevToken;

    foreach(QString token, parse(tExp))
    {
        // разруливаем унарный + и -
        if (prevToken == "("
                && (token == "+" || token == "-"))
            operands.push("0"); // Добавляем нулевой элемент

        if(isOperand(token)) // Если операнд
            operands.push(token); // то просто кидаем в стек

        else if(isFunction(token)
                || isBracket(token))
        {
            if(token == ")")
            {
                // Скобка - исключение из правил. выталкивает
                // все операции до первой открывающейся
                while(functions.size() > 0
                      && functions.last() != "(")
                    popFunction(&operands, &functions);

                functions.pop(); // Удаляем саму скобку "("

            }else
            {
                while(canPop(token, &functions)) // Если можно вытолкнуть
                    popFunction(&operands, &functions); // то выталкиваем

                functions.push(token); // Кидаем новую операцию в стек
            }
        }

        prevToken = token;

        if(debug)
        {
            qDebug() << token;
            qDebug() << operands;
            qDebug() << functions;
            qDebug();
        }
    }

    if(operands.size() > 1
            || functions.size() > 0)
        qCritical() << "Ошибка в разборе выражения";

    if(debug)
    {
        qDebug() << operands;
        qDebug() << functions;
    }

    return operands.pop().toDouble();
}
void UCalculateParser::setExpressions(const QString expressions)
{
    validExpressions = isValidExpressions(expressions);

    if(!validExpressions)
    {
        qWarning() << "Ошибка: неправильное выражение";
        return;
    }

    d_expressions = expressions;
}
QString UCalculateParser::expressions()
{
    return d_expressions;
}


/// FUNCTION
bool isValidExpressions(const QString expressions)
{
    return !(expressions.isNull() || expressions.isEmpty());
}

bool isFunction(const QString f)
{
    return f == "-"
            || f == "+"
            || f == "/"
            || f == "*"
            || f == "^"
            || f == "%";
}
bool isFunction(const QChar f)
{
    return f == '-'
            || f == '+'
            || f == '/'
            || f == '*'
            || f == '^'
            || f == '%';
}
bool isOperand(const QString o)
{
    return o.contains(QRegExp("[0-9]+"));
}
bool isOperand(const QChar o)
{
    return o.isDigit();
}
bool isBracket(const QString s)
{
    return s == "(" || s == ")";
}
bool isBracket(const QChar s)
{
    return s == '(' || s == ')';
}

QStringList parse(QString expressions)
{
    QStringList tokens;
    QString token;

//    bool isDigit = false;

//    for(int i = 0; i < expressions.size(); i++)
//    {
//        QChar c = expressions.at(i);

//        isDigit = isOperand(c);

//        if(isDigit)
//            token.append(c);
//        else
//        {
//            tokens << token;
//            token.clear();
//        }

//        if(isFunction(c)
//                || isBracket(c))
//            tokens << c;
//    }

//    qDebug() << tokens;

    QTextDocument document(expressions);
    QTextCursor cursor(&document);

    QString regExp = regExpNumber
            + "|" + regExpMathFunction
            + "|" + regExpFunction
            + "|" + regExpBrackets;

    while(!cursor.isNull())
    {
        cursor = document.find(QRegExp(regExp), cursor);
        if(cursor.isNull())
            break;

        token = cursor.selectedText();

        if(!(token.isEmpty()
             && token.isNull()))
        {
            if(token.indexOf("asin") != -1) // TODO
            {
                token = token.remove(QRegExp("^asin|\\(|\\)"));
                token = QString::number(qAsin(token.toDouble()));

            }else if(token.indexOf("acos") != -1)
            {
                token = token.remove(QRegExp("^acos|\\(|\\)"));
                token = QString::number(qAcos(token.toDouble()));

            }else if(token.indexOf("atan") != -1)
            {
                token = token.remove(QRegExp("^atan|\\(|\\)"));
                token = QString::number(qAtan(token.toDouble()));

            }else if(token.indexOf("sin") != -1)
            {
                token = token.remove(QRegExp("^sin|\\(|\\)"));
                token = QString::number(qSin(token.toDouble()));

            }else if(token.indexOf("cos") != -1)
            {
                token = token.remove(QRegExp("^cos|\\(|\\)"));
                token = QString::number(qCos(token.toDouble()));

            }else if(token.indexOf("tan") != -1)
            {
                token = token.remove(QRegExp("^tan|\\(|\\)"));
                token = QString::number(qTan(token.toDouble()));

            }else if(token.indexOf("sqrt") != -1)
            {
                token = token.remove(QRegExp("^sqrt|\\(|\\)"));
                token = QString::number(qSqrt(token.toDouble()));

            }else if(token.indexOf("ln") != -1)
            {
                token = token.remove(QRegExp("^ln|\\(|\\)"));
                token = QString::number(qLn(token.toDouble()));

            }else if(token.indexOf("exp") != -1)
            {
                token = token.remove(QRegExp("^exp|\\(|\\)"));
                token = QString::number(qExp(token.toDouble()));

            }else if(token.indexOf("floor") != -1)
            {
                token = token.remove(QRegExp("^floor|\\(|\\)"));
                token = QString::number(qFloor(token.toDouble()));

            }else if(token.indexOf("ceil") != -1)
            {
                token = token.remove(QRegExp("^ceil|\\(|\\)"));
                token = QString::number(qCeil(token.toDouble()));

            }else if(token.indexOf("abs") != -1)
            {
                token = token.remove(QRegExp("^abs|\\(|\\)"));
                token = QString::number(qFabs(token.toDouble()));
            }

            tokens << token;
        }
    }
//    qDebug() << tokens;

    return tokens;
}
void popFunction(QStack <QString> *o, QStack <QString> *f)
{
    qreal b = o->pop().toDouble();
    qreal a = o->pop().toDouble();

    QString function = f->pop();

    if(function == "+")
        o->push(QString::number(a + b));

    else if(function == "-")
        o->push(QString::number(a - b));

    else if(function == "*")
        o->push(QString::number(a * b));

    else if(function == "/")
        o->push(QString::number(a / b));

    else if(function == "^")
        o->push(QString::number(qPow(a, b)));

    else if(function == "%")
        o->push(QString::number((int)a % (int)b));
}

bool canPop(QString op1, QStack <QString> *f)
{
    if(f->isEmpty())
        return false;

    int p1 = getPriority(op1);
    int p2 = getPriority(f->last());

    return p1 >= 0 && p2 >= 0 && p1 >= p2;
}

int getPriority(QString op)
{
    // не выталкивает сам и не дает вытолкнуть себя другим
    if(op == "(")
        return -1;

    else if(op == "*" || op == "/")
        return 1;

    else if(op == "+" || op == "-")
        return 2;

    else if(op == "^" || op == "%")
        return 1;

    else
        qCritical() << "Недопустимая операция";
}
QString removeSpace(QString s)
{
    return s.remove(" ");
}
