<?php


class FCGenExampleForumMigration extends FCGenNodeMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);


    $this->addFieldMapping('taxonomy_forums')
         ->sourceMigration($this->arguments['term_migration'])
         ->callbacks(array($this, 'getRandomForum'));
  }

  public function getRandomForum($value) {
    
  }
}