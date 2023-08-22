#pragma once
#include"qaddr_bundle.hpp"
#include"crypto/qslip10.hpp"
#include"client/qclient.hpp"
#include"client/qclientMQTT.hpp"
#include<QObject>

#include<QString>
#include<QByteArray>
#include <QtQml/qqmlregistration.h>

#include <QtCore/QtGlobal>
#if defined(WINDOWS_ACCOU)
# define MIDLE_EXPORT Q_DECL_EXPORT
#else
#define MIDLE_EXPORT Q_DECL_IMPORT
#endif

using namespace qiota;
using namespace qiota::qblocks;
using namespace qcrypto;

class MIDLE_EXPORT MidlePay : public QObject
{
	Q_OBJECT

        Q_PROPERTY(QString  address MEMBER m_address NOTIFY addressChanged)
        Q_PROPERTY(bool  pass MEMBER m_pass NOTIFY passChanged)
        Q_PROPERTY(QString message MEMBER m_message NOTIFY messageChanged)
        Q_PROPERTY(QUrl  nodeaddr MEMBER m_naddr NOTIFY naddrChanged)
        Q_PROPERTY(QString  initTag MEMBER m_initTag NOTIFY initTagChanged)
        Q_PROPERTY(quint64  amount MEMBER m_amount NOTIFY amountChanged)

		QML_ELEMENT
		QML_SINGLETON

	public:
        MidlePay(QObject *parent = nullptr);

signals:
        void addressChanged();
        void passChanged();
        void naddrChanged();
        void messageChanged();
        void initTagChanged();
        void amountChanged();
	private:
        bool checkTransfer(Node_output out);
        QString m_address,m_message,m_tag,m_initTag;
        QUrl m_naddr;
        bool m_pass;
        Client* rest_client;
        ClientMqtt * mqtt_client;
        quint64 m_amount;

};

