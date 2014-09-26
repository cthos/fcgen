# FCGen - Fake Content Generation

The goal of this module is to provide a basis for generating real-seeming content in a manner which is consistent and customizable.

## Intended Usage

This module is not designed to be used on its own. It extends migrate so that you as the end user can create migrations which are based off of fake sources,
so that you can create test content that accurately represents your use cases.

The reason behind this is so that a site can be tested with large amounts of real content, for performance and load testing.

## Notes

* This is a work in progress, and has not been thoroughly tested.
* Designed to work with Migrate 2.6 or Higher. I recommend the dev branch.

## How to Use

FCGen provides several base migration classes from which you can extend your content. It is intended to be used as a base for your own Migration module,
though fcgen_example can be used to generate some of the more common content types (or be used as an example).

### Getting started

First thing you will need to do is create a module based off of migrate. Info on the procedure can be found here: https://www.drupal.org/node/1006982 and
by looking at fcgen_example.

After that is done, you'll need to implement your own sub-classes, or use the base classes found in includes/. Sane defaults have been implemented
in those classes, and several (like FCGenUserMigration) can be used without modification.

See fcgen_example.migrate.inc for examples on how to craft a proper hook_migrate_api implementation.

### FCGen Migration options

* number_to_generate - Determines how many items to generate for a given migration
* driver - Determines the random number generator driver. See the *Drivers* section below.
* max_random_user_associations - When a content type needs users attached to it, this number determines the "maximum" number of items of a given migration
can be attached to a given user. (It's not really a maximum because the user could be selected more than once).
* max_random_parent_associations - Determines the maximum number of child items could be attached to a given parent (used for comments).
* no_parent_chance - The 1 - 100 odds that a given "item" will not be attached to any parent. 0 = always, 100 = never.
* user_migration - The machine name of the users migration. Defaults to "Users"
* driver_language - Determines the language of the fake content being generated. Defaults to "en_US"

### Drivers

FCGen is designed with [Faker](https://github.com/fzaninotto/Faker) as the default randomness generator, but has been written in such a way that
it could be replaced by something different if necessary. See drivers/common.inc and drivers/faker.inc for implementation details.

## fcgen_example module notes

This module makes use of features in order to provide an example "Food" node migration, including some options for randomly selecting things from a dropdown
and sub-classing node migrations for your own purposes.