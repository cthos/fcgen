<?php
/**
 * @file
 * fcgen_example_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function fcgen_example_feature_node_info() {
  $items = array(
    'fcgen_food' => array(
      'name' => t('FCGen Food'),
      'base' => 'node_content',
      'description' => t('Holds the example "Food" Migration.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
