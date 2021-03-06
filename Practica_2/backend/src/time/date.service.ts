import { HttpService, Injectable } from '@nestjs/common';
import { AxiosResponse } from 'axios';
import { Observable } from 'rxjs';
import { microcontrollerConfig } from 'src/microcontroller.config';
import { Time } from './time';

@Injectable()
export class DateService {
  constructor(private http: HttpService) {}

  /**
   * Returns the current time for a city somewhere else in the world
   * @param location Tuple of Continent/City for which you want to know the time
   */
  public findDate(location: string): Observable<AxiosResponse<Time>> {
    return this.http.get(`http://worldtimeapi.org/api/timezone/${location}`);
  }

  /**
   * Formats a given date to the spanish locale
   * @param time
   */
  public formatDate(time: Time): string {
    const options: Intl.DateTimeFormatOptions = {
      weekday: 'long',
      year: 'numeric',
      month: 'long',
      day: 'numeric',
      hour: 'numeric',
      minute: 'numeric',
      timeZone: time.timezone,
    };

    const date = new Date(time.utc_datetime).toLocaleDateString(
      'es-ES',
      options,
    );

    return date;
  }

  /**
   * Sends the formatted date as a response to the ESP8266 server so it can publish
   * the result in the output topic
   * @param date The formatted date to be send back to the ESP8266
   */
  public sendDateToMicrocontroller(
    date: string,
  ): Observable<AxiosResponse<any>> {
    const response = `response=${date}`;

    return this.http.post(`${microcontrollerConfig.url}/response`, response, {
      headers: {
        'content-type': 'application/x-www-form-urlencoded',
      },
    });
  }
}
