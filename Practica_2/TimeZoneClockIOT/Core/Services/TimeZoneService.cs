using System;
using System.Globalization;
using System.Threading.Tasks;
using TimeZoneClockIOT.Core.Ports;

namespace TimeZoneClockIOT.Core.Services
{
    public class TimeZoneService : ITimeZoneService
    {
        private readonly ITimeZoneAdapter _timeZoneAdapter;
        private readonly IMqttAdapter _mqttAdapter;

        public TimeZoneService(ITimeZoneAdapter timeZoneAdapter, IMqttAdapter mqttAdapter)
        {
            _timeZoneAdapter = timeZoneAdapter ?? throw new ArgumentNullException(nameof(timeZoneAdapter));
            _mqttAdapter = mqttAdapter ?? throw new ArgumentNullException(nameof(mqttAdapter));
        }

        public async Task<string> GetTimeZoneMessage(string zone)
        {
            var timeZone = await _timeZoneAdapter.GetTimeZone(zone);
            return ConvertDateTimeToString(timeZone.Datetime);
        }

        private string ConvertDateTimeToString(DateTime date)
        {
            var culture = new CultureInfo("es-ES");
            var myTI = culture.TextInfo;
            return $"{myTI.ToTitleCase(date.ToString("dddd", culture))}, {date.Day} de {myTI.ToTitleCase(date.ToString("MMMM", culture))} de {date.Year} -- {date.ToString("HH:mm")}";
        }
    }
}
