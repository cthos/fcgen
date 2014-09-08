<?php

class MigrateSourceFaker extends MigrateSource {
  protected $count;
  protected $faker;

  protected $fetchedRows = 0;

  /**
   * Gets the key schema:
   *
   * @return array
   */
  public static function getKeySchema() {
    return array(
      'primaryKey' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
  }

  public function __construct($number_to_generate = 1000, $options = array()) {
    parent::__construct($options);
    $this->count = $number_to_generate;

    if (isset($options['faker'])) {
      $this->faker = $options['faker'];
    } else {
      $this->faker = Faker\Factory::create();

      if (isset($options['seed'])) {
        $this->faker->seed($options['seed']);
      }
    }
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    return 'Faker';
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();

    return $fields;
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    return $this->count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   *
   * Doesn't need to do anything, as there's nothing to roll back to.
   *
   * @return void
   */
  public function performRewind() {

  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * Returns a copy of faker, because sorcery.
   *
   * @return object
   */
  public function getNextRow() {
    if ($this->fetchedRows >= $this->count) {
      return false;
    }

    $this->fetchedRows++;

    return new MigrateSourceFakerShim($this->faker);
  }
}

/**
 * Class MigrateSourceFakerShim
 *
 * This is a fake class because migrate requires things to extend from StdClass
 */
class MigrateSourceFakerShim extends StdClass {
  protected $faker;
  protected $pk;

  public function __construct($faker) {
    $this->faker = $faker;
  }

  public function __call($method, $args) {
    try {
      return call_user_func_array(array($this->faker, $method), $args);
    } catch (\Exception $e) {
      return null;
    }
  }

  public function __get($name) {
    // Special exception for primaryKey
    if ($name == 'primaryKey') {
      if (!isset($this->pk)) {
        $this->pk = $this->faker->unique()->randomNumber;
      }
      return $this->pk;
    }

    try {
      $val = $this->faker->{$name};
      return $val;
    } catch (\Exception $e) {
      return false;
    }
  }

  public function __isset($name) {
    if (strpos($name, 'migrate_') === FALSE) {
      return true;
    }
  }
}