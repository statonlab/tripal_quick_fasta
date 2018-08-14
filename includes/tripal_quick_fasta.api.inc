<?php

function tripal_quick_run_file_job($parent_type_id) {


  // $type_name = db_select('chado.cvterm', 't')->fields('t', ['name'])->condition('t.cvterm_id', $parent_type_id)->execute()->fetchField();

  $name = 'proteins_from_' . $parent_type_id . '.fasta';
  $path = 'public://' . $name;
  // $path = file_create_url($uri);
  $prot_term = chado_get_cvterm(['id' => 'SO:0000104']);
  $query = db_select('chado.feature', 'f')
    ->fields('f', ['feature_id']);
  $query->join('chado.feature_relationship', 'fr', 'f.feature_id = fr.subject_id');
  $query->join('chado.feature', 'ftwo', 'ftwo.feature_id = fr.object_id');
  $query->condition('f.type_id', $prot_term->cvterm_id);
  $query->condition('ftwo.type_id', $parent_type_id);
  $count = $query->countQuery()->execute()->fetchField();

  $n = 500;
  $offset = 0;
  print("creating FASTA file for $count sequences.  This may take a while\n");
  while ($offset < $count) {

    if ($offset % ($n * 20) == 0) {
      print ("sequence number " . $offset . "\n");
    }
    $chunk = $query->range($offset, $n)->execute()->fetchAll();

    tripal_quick_get_protein_seqs_mRNA_names($chunk, $path, $parent_type_id);
    $offset += $n;
  }
}

/**
 * For a given chunk, write the feature sequence with the parent's header.
 *
 * @param $protein_feature_chunk
 * @param $path
 * @param $parent_type_id
 */
function tripal_quick_get_protein_seqs_mRNA_names($protein_feature_chunk, $path, $parent_type_id) {

  $fh = fopen(drupal_realpath($path), "w");
  foreach ($protein_feature_chunk as $item) {
    $feature_id = $item->feature_id;
    try {
      $seq = tripal_quick_fasta_fetch_seq($feature_id);
      $name = tripal_quick_fasta_fetch_parent_feature($feature_id, $parent_type_id);

      if ($seq) {
        fwrite($fh, ">" . $name . "\n" . $seq . "\n");
      }

    } catch (Exception $e) {
      print($e);
      return;
    }

  }
  fclose($fh);
}

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
 * given a feature, fetch the parent's uniquename.
 * note: feature should only have one parent of type mRNA and/or mrna_contig
 *
 * @param $feature_id
 *
 * @return mixed
 */
function tripal_quick_fasta_fetch_parent_feature($feature_id, $parent_type_id) {

  $query = db_select('chado.feature_relationship', 'fr');
  $query->join('chado.feature', 'f', 'fr.object_id = f.feature_id');
  $query->fields('f', ['uniquename']);
  $query->condition('fr.subject_id', $feature_id);
  $query->condition('f.type_id', $parent_type_id);
  $result = $query->execute()->fetchField();
  return $result;
}

function tripal_quick_fasta_fetch_seq($feature_id) {

  $result = db_select('chado.feature', 'f')
    ->fields('f', ['residues'])
    ->condition('f.feature_id', $feature_id)
    ->execute()->fetchField();

  if (!$result) {
    tripal_report_error('tripal', TRIPAL_WARNING, "No residues for feature ID '%feature'.",
      ['%feature' => $feature_id]);
  }
  return $result;
}

function tripal_quick_fasta_get_parent_feature_types() {

  $query = db_select('chado.feature', 'f');
  $query->join('chado.cvterm', 'cvt', 'f.type_id = cvt.cvterm_id');
  $query->fields('cvt', ['cvterm_id', 'name']);
  $query->distinct();

  $results = $query->execute()->fetchAll();

  $out = [];

  foreach ($results as $result) {
    $out[$result->cvterm_id] = $result->name;
  }
  return $out;
}