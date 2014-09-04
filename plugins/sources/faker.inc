<?php

use Faker;

class MigrateSourceFaker extends MigrateSource {
  protected $count;
  protected $faker;

  public function __construct($number_to_generate = 1000, $options = array()) {
    parent::__construct($options);
    $this->count = $number_to_generate;

    if (isset($options['faker'])) {
      $this->faker = $options['faker'];
    } else {
      $this->faker = Faker\Factory::create();
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
   * Does nothing because Faker doesn't need to be pushed forward.
   *
   * TODO: Return values correlating to the Possible Faker Options
   *
   * @return object
   */
  public function getNextRow() {
    return $this->faker;
  }
}