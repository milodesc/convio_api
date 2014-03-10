Description
----------
This is a drupal 7 module for interacting with Convio Luminate Online's API. By itself, this module does nothing useful aside from allowing a site administrator to configure and test a site's connection to the Convio API.

Its real usefulness is in providing functions to module developers that implement the Convio API. Currently, just 2 of the many Convio API functions have been implemented: createOrUpdate and isEmailValid. However, module developers may make any type of Convio API call via the more generic convio_api_request function.

Usage
-----
1) Configure your Convio account to accept API requests according to Convio Luminate Online's instructions (http://open.convio.com/api/#main.site_configuration.html)

2) Log in as a site administrator, install the module and visit Administration > Configuration > System > Convio API Configuration

3) Enter the information from step 1 in the configuration form amd save your changes

4) Test your connection and configuration via the Convio API Configuration Test

5) Download and install modules that use the API (e.g. https://github.com/milodesc/convio_signup) or create your own Convio API modules

6) Log bugs and contribute improvements to this module!