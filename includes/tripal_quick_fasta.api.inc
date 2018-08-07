<?php

function tripal_quick_fasta_fetch_features_for_sequence() {

  $query = db_select('chado.feature', 'f');
  $query->join('chado.cvterm', 'cvt', 'f.type_id = cvt.cvterm_id');
  $results = $query->fields('cvt', ['cvterm_id', 'name'])
    ->distinct()
    ->execute()
    ->fetchAll();

  $types = [];
  foreach ($results as $result) {
    $types[$result->cvterm_id] = $result->name;
  }

  return $types;
}


/**
 * @param $feature_id
 *
 * @return mixed
 */
function tripal_quick_fasta_fetch_parent_mRNA($feature_id){
  $query = db_select('chado.feature_relationship', 'fr');
    $query ->join('chado.feature', 'f', 'fr.object_id = f.feature_id');
  $query->fields('f', ['uniquename']);
  $query->condition('fr.subject_id', $feature_id);
  $result = $query->execute()->fetchField();
  return $result;
}