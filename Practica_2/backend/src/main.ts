import { NestFactory } from '@nestjs/core';
import { MicroserviceOptions, Transport } from '@nestjs/microservices';
import { AppModule } from './app.module';
import { mqttConfig } from './mqtt.config';

/**
 * Creates the microservice for a given MQTT configuration
 */
async function bootstrap() {
  const app = await NestFactory.createMicroservice<MicroserviceOptions>(
    AppModule,
    {
      transport: Transport.MQTT,
      options: {
        username: mqttConfig.username,
        password: mqttConfig.password,
        url: mqttConfig.url,
      },
    },
  );
  app.listen(() => console.log(`Microservice up and running`));
}
bootstrap();
