#include"midlePay.hpp"
#include <QRandomGenerator>
#include <QCryptographicHash>

using namespace qencoding::qbech32::Iota;

quint32 getCode(void)
{
    return QRandomGenerator::global()->generate();
}


MidlePay::MidlePay(QObject *parent):QObject(parent),m_address("smr1qp9rtwlc00ksp0mvet8ugwvqu03ygzr8s3x77w3df9qw9srm3hwk2l0v9kf"),
    m_naddr("https://api.shimmer.network"),m_pass(false),rest_client(new Client(this)),mqtt_client(new ClientMqtt(this)),
    m_message("Waiting for the node"),m_amount(1000)
{

    connect(this,&MidlePay::initTagChanged,this,[=](){
        m_tag=m_initTag+QString::number(getCode());
    });
    QObject::connect(mqtt_client,&QMqttClient::stateChanged,this,[=]
    {
        if(mqtt_client->state()==QMqttClient::Connected)
        {

            auto resp=mqtt_client->get_outputs_unlock_condition_address("address/"+m_address);
            connect(resp,&ResponseMqtt::returned,this,[=](QJsonValue data)
            {
                if(checkTransfer({Node_output(data)}))
                {
                    m_message="Payment accepted";
                    emit messageChanged();

                    m_pass=true;
                    emit passChanged();
                    resp->deleteLater();
                }
            });

        }
    });
    connect(rest_client,&qiota::Client::stateChanged,this,[=]()
    {
        if(rest_client->state()==Client::ClientState::Connected)
        {
            auto info=rest_client->get_api_core_v2_info();
            QObject::connect(info,&Node_info::finished,this,[=]( ){
                m_message="Transfer "+ QString::number(m_amount) + info->subunit + " to\n"+ m_address + "\n With tag:" + m_tag ;
                emit messageChanged();

            });

        }
    });
    mqtt_client->set_node_address(m_naddr);
    rest_client->set_node_address(m_naddr);

}
bool MidlePay::checkTransfer(Node_output out)
{

    const auto output=out.output();

    if(output->type()==Output::Basic_typ)
    {
        const auto tagfeature=output->get_feature_(Feature::Tag_typ);
        if(tagfeature)
        {
            const auto tag=std::static_pointer_cast<const Tag_Feature>(tagfeature)->tag();
            if(tag==m_tag)
            {
                auto ArrAddress= decode(m_address).second;
                const auto RecAddress=Address::from_array(ArrAddress);
                auto addr_bundle=new AddressBundle(RecAddress);
                auto outs=std::vector<Node_output>{out};
                addr_bundle->consume_outputs(outs);
                if(addr_bundle->amount>=m_amount)
                {
                    return true;
                }
            }
        }
    }
    return false;

}
