using System.Threading.Tasks;
using TimeZoneClockIOT.Infrastructure.Model;

namespace TimeZoneClockIOT.Core.Ports
{
    public interface ITimeZoneAdapter
    {
        Task<TimeZoneResponse> GetTimeZone(string zone);
    }
}
