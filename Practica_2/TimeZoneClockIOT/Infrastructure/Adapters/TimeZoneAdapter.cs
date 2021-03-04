using Microsoft.Extensions.Options;
using Newtonsoft.Json;
using System;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using TimeZoneClockIOT.Application.Settings;
using TimeZoneClockIOT.Infrastructure.Exceptions;
using TimeZoneClockIOT.Infrastructure.Model;
using TimeZoneClockIOT.Core.Ports;

namespace TimeZoneClockIOT.Infrastructure.Adapters
{
    public class TimeZoneAdapter : ITimeZoneAdapter
    {
        private readonly ApplicationSettings _settings;

        public TimeZoneAdapter(IOptions<ApplicationSettings> settings)
        {
            _settings = settings?.Value ?? throw new ArgumentNullException(nameof(settings));
        }
        public async Task<TimeZoneResponse> GetTimeZone(string zone)
        {
            var uri = new StringBuilder($"{_settings.HttpClientSettings.TimeZoneApi}/{zone}");
            try
            {
                using (var client = new HttpClient())
                {
                    var content = await client.GetStringAsync(new Uri(uri.ToString()));
                    return JsonConvert.DeserializeObject<TimeZoneResponse>(content);
                }
            }
            catch (Exception ex)
            {
                throw new InfrastructureException($"Has ocurred an exception calling World Time Api - {ex.Message}", ex);
            }
        }
    }
}
