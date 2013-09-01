<?php
/**
 * @file
 * Make and Delete Node Types from the Command line
 * See drush help --filter=field for how to make fields
 */

/**
 * Implementation of hook_drush_command().
 */
function node_type_make_drush_command() {
    $help = 'The name of the type in camelCase';
    $items['node-type-add'] = array(
        'description' => 'Make a Node type',
        'aliases' => array('nta'),
            'examples' => array(
              'drush entity-add newsArticle' => 'Will make a new Node/Entity machine name news_article human name News Article'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
           'type' => $help,
        )
    );

    $items['node-type-delete'] = array(
        'description' => 'Delete a Node type',
        'aliases' => array('ntd'),
        'examples' => array(
            'drush entity-delete newsArticle' => 'Will delete the Node/Entity type News Article'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
            'type' => $help,
        )
    );
  return $items;
}

/**
 * Command Callback. Add a node type.
 */

function drush_node_type_make_node_type_add() {
    $type = func_get_arg(0);
    $type_machine = preg_replace('/(?<=\\w)(?=[A-Z])/',"_$1", $type);
    $type_machine = strtolower($type_machine);
    $type_human = from_camel_case_to_machine_name($type);
    $info = new stdClass();
    $info->type = $type_machine;
    $info->name = $type_human;
    $info->base = 'node_content';
    $info->description = $type_human;
    $info->custom = TRUE;
    $info->locked = FALSE;
    $node_type = node_type_save($info);
    drush_log(dt("Note type made !type", array('!type' => print_r($node_type, 1))));
}

function drush_node_type_make_node_type_delete() {
    $type = func_get_arg(0);
    $type_machine = preg_replace('/(?<=\\w)(?=[A-Z])/',"_$1", $type);
    $type_machine = strtolower($type_machine);
    $node_type = node_type_delete($type_machine);
    drush_log(dt("Note !type deleted ", array('!type' => print_r($node_type, 1))));
}

/**
 * http://www.paulferrett.com/2009/php-camel-case-functions/
 * Translates a camel case string into a string with underscores (e.g. firstName -&gt; first_name)
 * @param    string   $str    String in camel case format
 * @return    string            $str Translated into underscore format
 */
function from_camel_case($str) {
    $str[0] = strtolower($str[0]);
    $func = create_function('$c', 'return "_" . strtolower($c[1]);');
    return preg_replace_callback('/([A-Z])/', $func, $str);
}

/**
 * Translates a string with underscores into camel case (e.g. first_name -&gt; firstName)
 * @param    string   $str                     String in underscore format
 * @param    bool     $capitalise_first_char   If true, capitalise the first char in $str
 * @return   string                              $str translated into camel caps
 */
function to_camel_case($str, $capitalise_first_char = false) {
    if($capitalise_first_char) {
        $str[0] = strtoupper($str[0]);
    }
    $func = create_function('$c', 'return strtoupper($c[1]);');
    return preg_replace_callback('/_([a-z])/', $func, $str);
}

/**
 * Custom script take from above to make machine name
 */

function from_camel_case_to_machine_name($str){
  $str[0] = strtoupper($str[0]);
  $func = create_function('$c', 'return " " . $c[1];');
  return preg_replace_callback('/([A-Z])/', $func, $str);
}
