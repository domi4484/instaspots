/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
 * TODO remove this class (include all this in user)               *
 *                                                                 *
********************************************************************/

#ifndef COMMAND_CANREGISTER_H
#define COMMAND_CANREGISTER_H

// Project includes ------------------------
#include "WebApiCommand.h"

class Command_CanRegister : public WebApiCommand
{
    Q_OBJECT
public:

    explicit Command_CanRegister(QObject *parent = 0);

    void execute(const QString &username);

    bool can();

signals:

public slots:

private:

    static const QString COMMAND;
    static const QString R_PARAM_USERNAME;
    static const QString A_PARAM_CANREGISTER;

    virtual QString command();

    virtual void setResult(const WebApiError &error,
                           const QScriptValue &result);


    bool m_Running;
    bool m_Can;
};

#endif // COMMAND_CANREGISTER_H
