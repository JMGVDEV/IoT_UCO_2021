using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Options;
using System;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using TimeZoneClockIOT.Application.Settings;
using TimeZoneClockIOT.Core.Ports;

namespace TimeZoneClockIOT.Application.Tasks
{
    public class TimeZoneTask : BackgroundService
    {
        private readonly ILogger<TimeZoneTask> _logger;
        private readonly ITimeZoneService _timeZoneService;
        private readonly IMqttAdapter _mqttAdapter;
        private readonly ApplicationSettings _settings;
        private static readonly string TOPIC_INPUT = "BackendInput";
        private static readonly string TOPIC_OUTPUT = "BackendOutput";
        private static readonly string TOPIC_JSONSTATUS = "JsonStatus";
        private static readonly string TOPIC_STATUSREQUEST= "StatusRequest";

        public TimeZoneTask(ILogger<TimeZoneTask> logger, ITimeZoneService timeZoneService, IMqttAdapter mqttAdapter, IOptions<ApplicationSettings> settings)
        {
            _logger = logger ?? throw new ArgumentNullException(nameof(logger));
            _timeZoneService = timeZoneService ?? throw new ArgumentNullException(nameof(timeZoneService));
            _mqttAdapter = mqttAdapter ?? throw new ArgumentNullException(nameof(mqttAdapter));
            _settings = settings?.Value ?? throw new ArgumentNullException(nameof(settings));
        }

        protected async override Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation($"{nameof(TimeZoneTask)} is starting");
            stoppingToken.Register(() => _logger.LogInformation($"{nameof(TimeZoneTask)} is stopping"));

            var factory = new MqttFactory();
            var mqttClient = factory.CreateMqttClient();

            var options = new MqttClientOptionsBuilder()
                .WithTcpServer(_settings.MqttClientSettings.Server, _settings.MqttClientSettings.Port)
                .WithCredentials(_settings.MqttClientSettings.User, _settings.MqttClientSettings.Password)
                .Build();
            await mqttClient.ConnectAsync(options, stoppingToken);

            await mqttClient.SubscribeAsync(TOPIC_INPUT);


            mqttClient.UseApplicationMessageReceivedHandler(async e =>
            {
                var message = string.Empty;
                message = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
                if (!string.IsNullOrEmpty(message))
                {
                    await ExecuteService(message);
                }
            });
        }

        private async Task ExecuteService(string message)
        {
            try
            {
                await _mqttAdapter.SendMessage($"Processing {message}", TOPIC_JSONSTATUS);
                await _mqttAdapter.SendMessage(await _timeZoneService.GetTimeZoneMessage(message), TOPIC_OUTPUT);
                await _mqttAdapter.SendMessage("200 - Ok", TOPIC_STATUSREQUEST);
            }
            catch (Exception ex)
            {
                await _mqttAdapter.SendMessage(ex.Message, TOPIC_STATUSREQUEST);
                await _mqttAdapter.SendMessage($"Error Processing {message}", TOPIC_JSONSTATUS);
            }
        }
    }
}
