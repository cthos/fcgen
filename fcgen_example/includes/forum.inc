<?php


class FCGenExampleForumMigration extends FCGenNodeMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this_ref =& $this;

    $this->addFieldMapping('taxonomy_forums', 'randomNumber')
         ->callbacks(function($value) use (&$this_ref) {
          return $this_ref->getRandomForum($value);
        });

    $this->addFieldMapping('title', 'catchPhrase', FALSE);

    $this->addFieldMapping('taxonomy_forums:source_type')->defaultValue('tid');
  }

  public function getRandomForum($value) {
    // Intentionally allowing random distribution.
    $migration = Migration::getInstance($this->arguments['term_migration']);
    $map_table = $migration->getMap()->getMapTable();

    $random_source_id = db_select($map_table, 'm')
      ->fields('m', array('destid1'))
      ->orderRandom()
      ->range(0,1)
      ->execute()
      ->fetchField();

    return $random_source_id;
  }
}

class FCGenExampleForumTermMigration extends FCGenTaxonomyMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('name', 'bs', FALSE);
  }
}

class FCGenExampleForumCommentMigration extends FCGenCommentMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);
  }
}
