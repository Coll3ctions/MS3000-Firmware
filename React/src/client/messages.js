export default {
  en: {
    header: {
      h1Html: 'MagicShifter',
    },
    menu: {
      home: 'Home',
      leds: 'Leds',
      paint: 'Paint',
      login: 'Login',
      logout: 'Logout',
      settings: 'Settings',
    },
    footer: {
      copyright: '&copy; 2014-2015 MagicShifter.net',
    },
    forms: {
      auth: {
        hint: 'Hint: pass1',
        legend: 'Login / Sign Up',
        placeholder: {
          email: 'your@email.com',
          password: 'password',
        },
        wrongPassword: 'Wrong password',
      },
      settings: {
        server: {
          legend: 'Change webserver settings',
          placeholder: {
            hostname: 'webserver hostname',
            port: 'webserver port',
          },
        },
        accesspoint: {
          legend: 'Change accesspoint settings',
          placeholder: {
            ssid: 'ssid for the MagicShifter Network',
            password: 'Password',
          },
        },
      },
      leds: {
        legend: 'You can set Led Colors here',
      },
    },
    buttons: {
      cancel: 'Cancel',
      edit: 'Edit',
      save: 'Save',
      login: 'Login',
      signup: 'Sign up',
    },
    confirmations: {
      cancelEdit: `You have unsaved changes. Are you sure you want to cancel them?`,
    },
    pages: {
      home: {
        title: 'Home',
        header: 'Home',
        content: `
          This is the MagicShifter Application, it will help set up your
          MagicShifter and you can even use the {ledsLink} page
          to live edit the led colors as well as the
          {paintLink} to create and edit images.
        `,
      },
      leds: {
        title: 'Leds',
        header: 'Leds',
      },
      auth: {
        title: 'Login',
        header: 'Login',
      },
      settings: {
        title: 'Settings',
        header: 'Settings',
      },
      notFound: {
        continueMessage: 'Continue here please.',
        header: 'This page is not available',
        message: 'The link may be broken, or the page may have been removed.',
        title: 'Page Not Found',
      },
    },
    validation: {
      email: `Email address is not valid!`,
      password: `Password must contain at least {minLength} characters.`,
      required: `Please fill out {prop, select,
        email {email}
        password {password}
        other {'{prop}'}
      }.`,
      ssid: `ssid can only contain a combination of a-z, A-Z and 0-9`,
    },
    colorPicker: {
      add: {
        title: 'Save selected color',
        text: 'Save color',
      },
      remove: {
        title: 'Remove this color',
        text: 'x',
      },
    },
  },
};