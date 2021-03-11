using System.Threading.Tasks;

namespace TimeZoneClockIOT.Core.Ports
{
    public interface IMqttAdapter
    {
        Task SendMessage(string message, string topic);
    }
}
