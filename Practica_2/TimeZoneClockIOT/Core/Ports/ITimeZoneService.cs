using System.Threading.Tasks;

namespace TimeZoneClockIOT.Core.Ports
{
    public interface ITimeZoneService
    {
        Task<string> GetTimeZoneMessage(string zone);
    }
}
