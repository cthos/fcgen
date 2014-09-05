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
  }
}