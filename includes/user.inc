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

    $driver = $this->driver;

    $this->addFieldMapping('name', 'userName')->callbacks(function ($value) use ($driver) {
      return $driver->generate('unique', 'userName');
    });
    // Use safeEmail to ensure it doesn't go anywhere.
    $this->addFieldMapping('email', 'safeEmail');
    $this->addFieldMapping('pass', 'password');

    $this->addFieldMapping('status', 'numberBetween')->callbacks(function ($value) use ($driver) {
      return $driver->generate('numberBetween', 0, 1);
    });
  }

  public function prepare($obj, $row) {
    //var_dump($obj);
  }
}