# Créer un executable à partir de fichiers sources
#add_executable(test_1  test_1.cxx)
add_executable(test_point test_point.cxx)
add_executable(test_univers test_univers.cxx)
add_executable(test_interactions test_interactions.cxx)


# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
#target_link_libraries(test_1 particules)

target_link_libraries(test_point particules)
target_link_libraries(test_univers particules)
target_link_libraries(test_interactions particules)