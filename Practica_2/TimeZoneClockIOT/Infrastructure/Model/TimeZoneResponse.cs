using System;

namespace TimeZoneClockIOT.Infrastructure.Model
{
    public class TimeZoneResponse
    {
        public string Abbreviation { get; set; }
        public string Client_ip { get; set; }
        public DateTime Datetime { get; set; }
        public int Day_of_week { get; set; }
        public int Day_of_year { get; set; }
        public bool Dst { get; set; }
        public object Dst_from { get; set; }
        public int Dst_offset { get; set; }
        public object Dst_until { get; set; }
        public int Raw_offset { get; set; }
        public string Timezone { get; set; }
        public int Unixtime { get; set; }
        public DateTime Utc_datetime { get; set; }
        public string Utc_offset { get; set; }
        public int Week_number { get; set; }
    }
}
