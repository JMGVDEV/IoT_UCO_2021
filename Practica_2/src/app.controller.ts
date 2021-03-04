import { Controller, Inject } from '@nestjs/common';
import { ClientProxy, MessagePattern, Payload } from '@nestjs/microservices';
import { DateService } from './time/date.service';

@Controller()
export class AppController {
  constructor(
    @Inject('TIME_SERVICE') private client: ClientProxy,
    private timeService: DateService,
  ) {}

  /**
   * Connects to the broker during application startup and publishes a message upon
   * a successful connection
   */
  public async onApplicationBootstrap() {
    await this.client.connect();
    this.client.emit<string>('alive', 'Broker connection established');
  }

  /**
   * Subscribes to the `input` topic and tries to return the date for a given city
   * @param data Payload received from the message
   */
  @MessagePattern('input')
  public async handleTimeRequest(@Payload() data: string) {
    try {
      const response = await this.timeService.findDate(data).toPromise();
      const date = this.timeService.formatDate(response.data);

      this.client.emit<string>('jsonstatus', response.status);
      this.client.emit<string>('output', date);
    } catch (err) {
      this.client.emit<string>(
        'statusrequest',
        `An error ocurred attempting to get the date for ${data}: ${err.response.status}`,
      );
    }
  }
}
