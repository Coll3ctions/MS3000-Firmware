import React from 'react';
import {Route, IndexRoute, Redirect} from 'react-router';

import CoreLayout from 'layouts/CoreLayout';

import InfoView from 'views/Info';

import ImagesView from 'views/Images';
import EffectsView from 'views/Effects';
import FontView from 'views/Font';
import ColorView from 'views/Color';
import UploadView from 'views/Upload';

import SettingsView from 'views/Settings';
import NotFoundView from 'views/NotFound';

export default (
  <Route path='/' component={CoreLayout}>
    <IndexRoute />
    <Route
      path='/paint/images'
      component={ImagesView}
    />

    <Route
      path='/paint/font'
      component={FontView}
    />

    <Route
      path='/paint/effects'
      component={EffectsView}
    />
    <Route
      path='/paint/color'
      component={ColorView}
    />

    <Route
      path='/paint/upload'
      component={UploadView}
    />

    <Route path='/info' component={InfoView} />
    <Route path='/settings' component={SettingsView} />
    <Route path='/404' component={NotFoundView} />
    <Redirect from='*' to='/404' />
  </Route>
);
