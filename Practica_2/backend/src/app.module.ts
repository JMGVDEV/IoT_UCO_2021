import { HttpModule, Module } from '@nestjs/common';
import { ClientsModule, Transport } from '@nestjs/microservices';
import { AppController } from './app.controller';
import { mqttConfig } from './mqtt.config';
import { DateService } from './time/date.service';

@Module({
  imports: [
    /**
     * Register a client to pub/sub to a messaging broker
     */
    ClientsModule.register([
      {
        name: 'TIME_SERVICE',
        transport: Transport.MQTT,
        options: {
          username: mqttConfig.username,
          password: mqttConfig.password,
          url: mqttConfig.url,
        },
      },
    ]),
    HttpModule,
  ],
  controllers: [AppController],
  providers: [DateService],
})
export class AppModule {}
