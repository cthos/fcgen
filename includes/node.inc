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

    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->arguments['bundle']);

    // Do some default setup:
    $this->addFieldMapping('title', 'sentence')->callbacks(function ($value) use ($this) {
      return $this->faker->sentence(10);
    });
    $this->addFieldMapping('body', 'text');

    $this->addFieldMapping('uid')
         ->sourceMigration($this->arguments['user_migration'])
         ->callbacks(array($this, 'selectRandomUser'));
  }
}