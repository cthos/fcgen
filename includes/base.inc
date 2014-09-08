<?php
/**
 * @file Holds common classes for the FCGen migration.
 */

class FCGenBaseMigration extends Migration {
  protected $driver;

  protected $parent_dependency_counter;
  protected $parent_dependency_number;
  protected $random_parent;

  protected $user_dependency_counter;
  protected $user_dependency_number;
  protected $random_user_id;

  public function __construct($arguments) {
    // Generation defaults
    $defaults = array(
      'number_to_generate' => 1000,
      'driver' => 'faker',
      'max_random_user_associations' => 40,
      'max_random_parent_associations' => 100,
      'user_migration' => 'Users',
      'driver_language' => 'en_US',
    );

    $arguments = array_merge($defaults, $arguments);

    parent::__construct($arguments);

    $this->driver = FCGenDriver::factory($this->arguments['driver']);
    $this->driver->init(array(
      'language' => $this->arguments['driver_language'],
    ));
    $this->source = $this->driver->getSource($arguments['number_to_generate']);
  }

  /**
   * Grabs a random user out of the map.
   *
   * @todo This only works with MigrateSQLMap at the moment.
   */
  public function selectRandomUser($value) {
    if (!isset($this->user_dependency_number) || $this->user_dependency_counter >= $this->user_dependency_number) {
      $this->user_dependency_counter = 0;
      $this->user_dependency_number = $this->driver->generate('numberBetween', 1, $this->arguments['max_random_user_associations']);

      $user_migration = Migration::getInstance($this->arguments['user_migration']);
      $map_table = $user_migration->getMap()->getMapTable();

      $this->random_user_id = db_select($map_table, 'm')
        ->fields('m', array('destid1'))
        ->orderRandom()
        ->range(0,1)
        ->execute()
        ->fetchField();
    }

    $this->user_dependency_counter++;

    return $this->random_user_id;
  }

  /**
   * Grabs a random source id from whatever's in 'source_migration'
   *
   * @todo Make this work for more than one field, and able to handle the
   * random stacking.
   */
  public function selectRandomFromSource($value) {
    $migration = Migration::getInstance($this->arguments['source_migration']);
    $map_table = $migration->getMap()->getMapTable();

    $random_source_id = db_select($map_table, 'm')
      ->fields('m', array('sourceid1'))
      ->orderRandom()
      ->execute()
      ->fetchField();

    return $random_source_id;
  }

  /**
   * Finds a random parent from the source migration table.
   *
   * Does this by picking a number of children to be attached to a random parent
   * and then tacking the next x terms onto it as children.
   *
   * Should return 0 for the first parent created. 0 children for a given
   * "batch" is also possible.
   *
   * @todo Should I prevent it from selecting the same parent at random more
   * than once?
   */
  public function getRandomParent($value) {
    if (!isset($this->parent_dependency_counter) || $this->parent_dependency_counter >= $this->parent_dependency_number) {
      $this->parent_dependency_counter = 0;
      $this->parent_dependency_number = $this->driver->generate('numberBetween', 0, $this->arguments['max_random_parent_associations']);

      if ($this->parent_dependency_number == 0) {
        return NULL;
      }

      $term_migration = Migration::getInstance($this->machineName);
      $map_table = $term_migration->getMap()->getMapTable();

      $this->random_parent = db_select($map_table, 'm')
        ->fields('m', array('destid1'))
        ->orderRandom()
        ->range(0,1)
        ->execute()
        ->fetchField();
    }

    $this->parent_dependency_counter++;

    return $this->random_parent;
  }
}