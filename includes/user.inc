<?php
/**
 * @file User-migration related classes
 */

class FCGenUserMigration extends FCGenBaseMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationUser::getKeySchema()
    );

    $this->destination = new MigrateDestinationUser();

    $this->addFieldMapping('name', 'userName');
    // Use safeEmail to ensure it doesn't go anywhere.
    $this->addFieldMapping('email', 'safeEmail');
    $this->addFieldMapping('pass', 'password');

    $this->addFieldMapping('status')->callbacks(function ($value) {
      return $this->faker->numberBetween(0,1);
    });
  }
}