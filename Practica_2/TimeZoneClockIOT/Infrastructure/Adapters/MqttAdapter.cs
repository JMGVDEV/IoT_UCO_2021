using Microsoft.Extensions.Options;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Options;
using System;
using System.Threading;
using System.Threading.Tasks;
using TimeZoneClockIOT.Application.Settings;
using TimeZoneClockIOT.Core.Ports;


namespace TimeZoneClockIOT.Infrastructure.Adapters
{
    public class MqttAdapter : IMqttAdapter
    {
        private readonly ApplicationSettings _settings;
        private readonly IMqttClient _mqttClient;

        public MqttAdapter(IOptions<ApplicationSettings> settings)
        {
            _settings = settings?.Value ?? throw new ArgumentNullException(nameof(settings));
            _mqttClient = new MqttFactory().CreateMqttClient();
        }

        public async Task SendMessage(string message, string topic)
        {
            var options = new MqttClientOptionsBuilder()
               .WithTcpServer(_settings.MqttClientSettings.Server, _settings.MqttClientSettings.Port)
               .WithCredentials(_settings.MqttClientSettings.User, _settings.MqttClientSettings.Password)
               .Build();

            CancellationToken cancellationToken;
            await _mqttClient.ConnectAsync(options, cancellationToken);

            var payload = new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(message)
                .WithExactlyOnceQoS()
                .Build();

            await _mqttClient.PublishAsync(payload, cancellationToken);

            await _mqttClient.DisconnectAsync();
        }
    }
}
