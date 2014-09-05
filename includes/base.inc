<?php
/**
 * @file Holds common classes for the FCGen migration.
 */

use Faker;

class FCGenBaseMigration extends Migration {
  protected $driver;

  protected $user_dependency_counter;
  protected $user_dependency_number;
  protected $random_user_id;

  public function __construct($arguments) {
    // Generation defaults
    $defaults = array(
      'number_to_generate' => 1000,
      'driver' => 'faker',
      'max_random_user_associations' => 40,
      'user_migration' => 'Users',
    );

    $arguments = array_merge($defaults, $arguments);

    parent::__construct($arguments);

    $this->driver = FCGenDriver::factory($arguments['driver']);
    $this->source = $this->driver->getSource($arguments['number_to_generate']);
  }

  /**
   * Grabs a random user out of the map.
   *
   * @todo This only works with MigrateSQLMap at the moment.
   */
  public function selectRandomUser() {
    if (!isset($this->user_dependency_number) || $this->user_dependency_counter >= $this->user_dependency_number) {
      $this->user_dependency_counter = 0;
      $this->user_dependency_number = $this->driver->randomBetween(1, $this->arguments['max_random_user_associations']);

      $user_migration = Migration::getInstance($this->arguments['user_migration']);
      $map_table = $user_migration->getMap()->getMapTable();

      $this->random_user_id = db_select($map_table, 'm')
        ->fields('m', array('sourceid1'))
        ->orderRandom()
        ->execute()
        ->fetchField();
    }

    $this->user_dependency_counter++;

    return $this->random_user_id;
  }
}