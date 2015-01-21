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

#ifndef COMMAND_REGISTER_H
#define COMMAND_REGISTER_H

// Project includes ------------------------
#include "WebApiCommand.h"

// Forward declarations --------------------
class Command_Register : public WebApiCommand
{
    Q_OBJECT
public:

    explicit Command_Register(QObject *parent = 0);

    void execute(const QString &username,
                 const QString &password,
                 const QString &eMail);

    bool success();

signals:

public slots:

private:

    static const QString COMMAND;

    static const QString R_PARAM_USERNAME;
    static const QString R_PARAM_PASSWORD;
    static const QString R_PARAM_EMAIL;

    static const QString A_PARAM_REGISTERED;

    virtual QString command();

    virtual void setResult(const WebApiError &error,
                           const QScriptValue &result);


    bool m_Running;
    bool m_Success;

};

#endif // COMMAND_REGISTER_H
