<?php


class FCGenCommentMigration extends FCGenBaseMigration {

  public function __construct($arguments) {

    parent::__construct($arguments);

    if (empty($this->arguments['bundle'])) {
      throw new \Exception("Must specify bundle for node to which comments will be attached");
    }

    if (empty($this->arguments['source_migration'])) {
      throw new \Exception('Please pass source_migration as the machine name of the migration associated with the nodes to
       which these comments will be attached.');
    }

    $this->dependencies[] = $this->arguments['source_migration'];

    $this->destination = new MigrateDestinationComment($this->arguments['bundle']);

    $this->map = new MigrateSQLMap(
      $this->machineName,
      $this->driver->getKeySchema(),
      MigrateDestinationComment::getKeySchema()
    );

    $this->addFieldMapping('nid', 'nid')
         ->sourceMigration($this->arguments['source_migration']);

    $this->addFieldMapping('uid', 'uid')
         ->callbacks(array($this, 'selectRandomUser'));

    // TODO: This is a problem, because it needs to be within the thread.
    $this->addFieldMapping('pid', 'pid');

    $this->addFieldMapping('comment_body', 'text');
    $this->addFieldMapping('created', 'timestamp');
  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->pid = $this->getRandomParent(0);

    if ($row->pid) {
      $row->nid = $this->getNodeFromComment($row->pid);
    } else {
      // This has to be done here, since source mappings are applied before callbacks.
      $row->nid = $this->selectRandomFromSource(0);
    }
  }

  protected function getNodeFromComment($cid) {
    $nid = db_select('comment', 'c')
             ->fields('c', array('nid'))
             ->condition('c.cid', $cid)
             ->execute()
             ->fetchField();

    $migration = Migration::getInstance($this->arguments['source_migration']);
    $map = $migration->getMap();

    $sourceid = $map->lookupSourceID(array($nid));

    return $sourceid['sourceid1'];
  }
}