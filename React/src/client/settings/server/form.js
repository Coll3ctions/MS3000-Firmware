import {Record} from 'immutable';

// immutable Record is superuseful. It defines default values and getters.
const FormRecord = Record({
  fields: new (Record({
    hostname: 'magicshifter',
    port: '80'
  })),
  error: null
});

export default class Form extends FormRecord {
  // We can add getters here. For example:
  // get emailIsTheSameAsPassword() {
  //   return this.email === this.password;
  // }
}
