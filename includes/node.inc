<?php
/**
 * @file Holds the Faker node migration base.
 */

class FCGenNodeMigration extends FCGenBaseMigration {

  /**
   * Constructor.
   *
   * Required arguments:
   *  - bundle : the bundle name for the target node.
   *
   * @param $arguments
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    if (isset($this->arguments['user_migration'])) {
      $this->dependencies[] = $this->arguments['user_migration'];
    }

    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->arguments['bundle']);

    $driver = $this->driver;

    // Do some default setup:
    $this->addFieldMapping('title', 'sentence')->callbacks(function ($value) use ($driver) {
      return $driver->generate('sentence', 10);
    });
    $this->addFieldMapping('body', 'text');

    $this->addFieldMapping('uid', 'randomNumber')
         ->callbacks(array($this, 'selectRandomUser'));

    $this->addFieldMapping('created', 'timestamp');
  }
}