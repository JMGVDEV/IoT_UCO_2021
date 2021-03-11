using System;
using System.IO;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Serilog;
using TimeZoneClockIOT.Application.Settings;
using TimeZoneClockIOT.Application.Tasks;
using TimeZoneClockIOT.Core.Ports;
using TimeZoneClockIOT.Core.Services;
using TimeZoneClockIOT.Infrastructure.Adapters;

namespace TimeZoneClockIOT
{
    public class Program
    {
        private static IConfiguration Configuration;

        protected Program() { }

        public static void Main(string[] args)
        {
            GetConfigurations(args);
            CreateHostBuilder(args).Build().Run();
        }

        private static void GetConfigurations(string[] args)
        {
            Configuration = new ConfigurationBuilder()
               .SetBasePath(Directory.GetCurrentDirectory())
               .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
               .AddJsonFile($"appsettings.{Environment.GetEnvironmentVariable("ASPNETCORE_ENVIRONMENT")}.json", optional: true, reloadOnChange: true)
               .AddEnvironmentVariables(prefix: "APP_")
               .AddCommandLine(args)
               .Build();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureAppConfiguration((host, config) => 
                {
                    config.AddConfiguration(Configuration);
                })
                .ConfigureServices((hostContext, services) =>
                {
                    services.Configure<ApplicationSettings>(Configuration);
                    services.AddTransient<ITimeZoneAdapter, TimeZoneAdapter>();
                    services.AddTransient<ITimeZoneService, TimeZoneService>();
                    services.AddTransient<IMqttAdapter, MqttAdapter>();
                    services.AddHostedService<TimeZoneTask>();


                    Log.Logger = new LoggerConfiguration()
                        .MinimumLevel.Information()
                        .ReadFrom.Configuration(hostContext.Configuration)
                        .WriteTo.Console()
                        .CreateLogger();
                });
    }
}
