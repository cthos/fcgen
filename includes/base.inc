<?php
/**
 * @file Holds common classes for the FCGen migration.
 */

class FCGenBaseMigration extends Migration {
  protected $driver;

  protected $parent_dependency_counter;
  protected $parent_dependency_number;
  protected $random_parent;

  protected $random_user_id;

  protected $_selected_users = array();

  public function __construct($arguments) {
    // Generation defaults
    $defaults = array(
      'number_to_generate' => 1000,
      'driver' => 'faker',
      'max_random_user_associations' => 40,
      'max_random_parent_associations' => 100,
      'no_parent_chance' => 75,
      'user_migration' => 'Users',
      'driver_language' => 'en_US',
      'duplicate_user_chance' => 20,
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
    if ($this->arguments['duplicate_user_chance']) {
      if ($this->driver->generate('numberBetween', 1, 100) <= $this->arguments['duplicate_user_chance']) {
        while (TRUE) {
          $user = array_rand($this->_selected_users);

          if ($this->_selected_users[$user] > 0) {
            $this->random_user_id = $user;
            return $this->random_user_id;
          }
        }
      }
    }

    while (TRUE) {
      $this->_findRandomUser();

      if (empty($this->_selected_users[$this->random_user_id])) {
        $dep_number = $this->driver->generate('numberBetween', 1, $this->arguments['max_random_user_associations']);
        $this->_selected_users[$this->random_user_id] = $dep_number;
      }

      if ($this->_selected_users[$this->random_user_id] > 0) {
        break;
      }
    }

    $this->_selected_users[$this->random_user_id] -= 1;

    return $this->random_user_id;
  }

  /**
   * Grabs a random user out of the map table's database.
   */
  protected function _findRandomUser() {
    $user_migration = Migration::getInstance($this->arguments['user_migration']);
    $map_table = $user_migration->getMap()->getMapTable();

    $this->random_user_id = db_select($map_table, 'm')
      ->fields('m', array('destid1'))
      ->orderRandom()
      ->range(0,1)
      ->execute()
      ->fetchField();

    return $this->random_user_id;
  }

  /**
   * Grabs a random source id from whatever's in 'source_migration'
   *
   * @todo Make this work for more than one field, and able to handle the
   * random stacking.
   */
  public function selectRandomFromSource($value) {
    if (!empty($value)) {
      return $value;
    }

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
    if (!$this->arguments['max_random_parent_associations']) {
      return NULL;
    }

    if ($this->arguments['no_parent_chance']) {
      $parent_needed = $this->driver->generate('numberBetween', 1, 100);

      if ($parent_needed <= $this->arguments['no_parent_chance']) {
        return NULL;
      }
    }

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

  /**
   * Finds a random comment in the node to attach to a parent.
   *
   * @param int $attached_entity_id
   *   The entity to which we're tacking on this comment.
   * @param float $no_parent_chance
   *   The probability that a given comment will have no parent.
   *
   * @return int|null
   */
  public function getRandomCommentParent($attached_entity_id, $no_parent_chance = 0.5) {
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'comment')
          ->entityCondition('nid', $attached_entity_id)
          ->addTag('efq_random')
          ->range(0, 1);

    $result = $query->execute();

    if (empty($result['comment'])) {
      return NULL;
    }

    return current(array_keys($result['comment']));
  }
}