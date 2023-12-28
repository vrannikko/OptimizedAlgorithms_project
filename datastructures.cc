// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_affiliation_count()
{
    return affIDList.size();
}

void Datastructures::clear_all()
{
    affIDList.clear();
    affiliations_map.clear();
    pubIDList.clear();
    publications_map.clear();
    coord_to_id_map.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return affIDList;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    if ( std::find(affIDList.begin(), affIDList.end(), id) != affIDList.end() ) {
        return false;
    }

    // Initialize
    affIDList.push_back(id);
    AffiliationData newAff;
    newAff.name = name;
    newAff.coords = xy;
    affiliations_map.insert({id, newAff});
    coord_to_id_map[xy] = id;
    alphabetical = false;
    distance_increasing = false;
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto i = affiliations_map.find(id);
    if (i == affiliations_map.end()) {
        return NO_NAME;
    }
    return i->second.name;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto i = affiliations_map.find(id);
    if (i == affiliations_map.end()) {
        return NO_COORD;
    }
    return i->second.coords;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    //Not re-sorting if already in correct order.
    if ( alphabetical ) { return affIDList; }

    const auto& a = affiliations_map;
    std::sort(affIDList.begin(), affIDList.end(),
              [&a](auto const& i1, auto const& i2)
              { return (a.at(i1).name < a.at(i2).name); });

    //Update flags
    alphabetical = true;
    distance_increasing = false;

    return affIDList;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    //Not re-sorting if already in correct order.
    if ( distance_increasing ) { return affIDList; }

    const auto& a = affiliations_map;
    std::sort(affIDList.begin(), affIDList.end(),
              [&a](auto const& i1, auto const& i2)
    {
        Coord coord1 = a.at(i1).coords;
        Coord coord2 = a.at(i2).coords;

        //d = sqrt(x^2+y^2) -> d^2=x^2+y^2 (Euclidian distance)
        //Non-squared distances suffice in the comparison. (for efficiency)

        int dist1 = coord1.x * coord1.x + coord1.y * coord1.y;
        int dist2 = coord2.x * coord2.x + coord2.y * coord2.y;

        if ( dist1 != dist2 ) { return dist1 < dist2; }
        return coord1.y < coord2.y; });

    //Update flags
    alphabetical = false;
    distance_increasing = true;

    return affIDList;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto i = coord_to_id_map.find(xy);
    if (i == coord_to_id_map.end()) { return NO_AFFILIATION; }

    return i->second;

   // Other version in case:
   // for ( const auto& a : affiliations_map ) {
   //     if ( a.second.coords == xy ) { return a.first; }
   // }
   // return NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    //Deleting old from coord_to_id_map and changing .coords to affiliations_map
    auto i = affiliations_map.find(id);
    if (i == affiliations_map.end()) { return false; }
    auto xy = i->second.coords;
    i->second.coords = newcoord;
    auto i2 = coord_to_id_map.find(xy);
    coord_to_id_map.erase(i2);
    coord_to_id_map[newcoord] = id;

    distance_increasing = false;
    return true;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), id) != pubIDList.end() ) {
        return false;
    }

    for ( const auto& a : affiliations ) {
        affiliations_map.at(a).related_pubs.push_back(id);
    }

    //Initialize
    pubIDList.push_back(id);
    PublicationData newPub;
    newPub.name = name;
    newPub.year = year;
    newPub.parent = NO_PUBLICATION;
    publications_map.insert({id, newPub});

    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    return pubIDList;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        return NO_NAME;
    }

    return publications_map.at(id).name;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        return NO_YEAR;
    }

    return publications_map.at(id).year;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        std::vector<AffiliationID> v;
        v.push_back(NO_AFFILIATION);
        return v;
    }
    return publications_map.at(id).related_affs;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    //Can't add reference, if either ID doesn't have a publication.
    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        return false; }

    if ( std::find(pubIDList.begin(), pubIDList.end(), parentid) == pubIDList.end() ) {
        return false; }

    publications_map.at(parentid).references.push_back(id);
    publications_map.at(id).parent = parentid;
    return true;

}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{

    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        std::vector<PublicationID> reference_IDs;
        reference_IDs.push_back(NO_PUBLICATION);
        return reference_IDs;
    }

    return publications_map.at(id).references;
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), publicationid) == pubIDList.end() ) {
        return false; }

    if ( std::find(affIDList.begin(), affIDList.end(), affiliationid) == affIDList.end() ) {
        return false; }

    //Add publication and affiliation to eachother.
    affiliations_map.at(affiliationid).related_pubs.push_back(publicationid);
    publications_map.at(publicationid).related_affs.push_back(affiliationid);
    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    if ( std::find(affIDList.begin(), affIDList.end(), id) == affIDList.end() ) {
        std::vector<PublicationID> v;
        v.push_back(NO_PUBLICATION);
        return v;
    }

    return affiliations_map.at(id).related_pubs;
}

PublicationID Datastructures::get_parent(PublicationID id)
{
   //No publication found
   auto i = publications_map.find(id);
   if (i == publications_map.end()) { return NO_PUBLICATION; }
   const auto& temp_parent = publications_map.at(id).parent;
   return temp_parent;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    std::vector<std::pair<Year, PublicationID>> year_and_pub;

    //Returning empty pair, if no IDs found.
    if ( std::find(affIDList.begin(), affIDList.end(), affiliationid) == affIDList.end() ) {
        std::pair<Year, PublicationID> empty_pair = std::make_pair(NO_YEAR, NO_PUBLICATION);
        year_and_pub.push_back(empty_pair);
        return year_and_pub;
    }

    //Sort by year, or if same year, by name
    const auto& p_map = publications_map;
    auto& p_vec = affiliations_map.at(affiliationid).related_pubs;
    std::sort(p_vec.begin(), p_vec.end(),
              [&p_map](auto const& i1, auto const& i2)
    {
        if ( p_map.at(i1).year == p_map.at(i2).year ) { return p_map.at(i1).name < p_map.at(i2).name; }
        return p_map.at(i1).year < p_map.at(i2).year;
    });

    //Push back values to vector, compare to given year value
    for ( const auto& pub : p_vec ) {
        auto current_year = p_map.at(pub).year;
        if ( current_year >= year) {
            std::pair<Year, PublicationID> newpair = std::make_pair(current_year, pub);
            year_and_pub.push_back(newpair);
        }
    }
    return year_and_pub;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> parentChain;

    //Can't find ID
    auto i = publications_map.find(id);
    if (i == publications_map.end()) {
        parentChain.push_back(NO_PUBLICATION);
        return parentChain;
    }

    //No parents on current ID
    if ( publications_map.at(id).parent == NO_PUBLICATION ) { return parentChain; }

    //Append all parents to parentChain
    auto temp_id = get_parent(id);

    while (temp_id != NO_PUBLICATION) {
        parentChain.push_back(temp_id);
        temp_id = get_parent(temp_id);
    }

    return parentChain;

   // Other code snippets just in case:
   // if ( get_parent(id) != NO_PUBLICATION ) {
   //     const auto& current_parent = publications_map.at(id).parent;
   //     //Find parent(s) recursively
   //     auto innerParentChain = get_referenced_by_chain(current_parent);
   //     parentChain.insert(parentChain.end(), innerParentChain.begin(), innerParentChain.end());
   //     return parentChain;
   // }
   // else {
   //     parentChain.push_back(NO_PUBLICATION);
   //     return parentChain;
   // }
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> all_references;

    if ( std::find(pubIDList.begin(), pubIDList.end(), id) == pubIDList.end() ) {
        all_references.push_back(NO_PUBLICATION);
        return all_references;
    }

    //Fetch and push_back to all_references all the references of references.
    const auto& r = publications_map.at(id).references;
    for ( const auto& ref : r ) {
        std::vector<PublicationID> ref_of_ref = get_all_references(ref);
        all_references.insert(all_references.end(), ref_of_ref.begin(), ref_of_ref.end());
    }

    all_references.insert(all_references.end(), r.begin(), r.end());
    return all_references;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_affiliations_closest_to()");
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    // Replace the line below with your implementation
    // throw NotImplemented("remove_affiliation()");

    if ( std::find(affIDList.begin(), affIDList.end(), id) == affIDList.end() ) {
        return false;
    }

    std::vector<PublicationID> pubs_of_id = affiliations_map.at(id).related_pubs;

    //Delete mention of affiliation from all of it's publications
    for ( const auto& pub : pubs_of_id ) {
        auto& affs_of_pub = publications_map.at(pub).related_affs;
        affs_of_pub.erase(std::remove(affs_of_pub.begin(), affs_of_pub.end(), id), affs_of_pub.end());
    }

    //Delete from affIDList
    affIDList.erase(std::remove(affIDList.begin(), affIDList.end(), id), affIDList.end());

    //Delete from coord_to_id_map
    auto xy = affiliations_map.at(id).coords;
    auto i = coord_to_id_map.find(xy);
    coord_to_id_map.erase(i);

    //Delete from affiliations_map
    auto i2 = affiliations_map.find(id);
    affiliations_map.erase(i2);

    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    if ( std::find(pubIDList.begin(), pubIDList.end(), id1) == pubIDList.end() ) {
        return NO_PUBLICATION;
    }

    if ( std::find(pubIDList.begin(), pubIDList.end(), id2) == pubIDList.end() ) {
        return NO_PUBLICATION;
    }

    std::unordered_set<PublicationID> id1_parents;
    id1 = publications_map.at(id1).parent;
    id2 = publications_map.at(id2).parent;

    //List all id1's parents
    while ( id1 != NO_PUBLICATION ) {
        id1_parents.insert(id1);
        id1 = publications_map.at(id1).parent;
    }

    //Compare every parent of id1 to id2's parents
    while ( id2 != NO_PUBLICATION ) {
        if (id1_parents.find(id2) != id1_parents.end()) { return id2; }
        id2 = publications_map.at(id2).parent;
    }

    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{

    if ( std::find(pubIDList.begin(), pubIDList.end(), publicationid) == pubIDList.end() ) {
        return false;
    }

    std::vector<AffiliationID> affs_of_id = publications_map.at(publicationid).related_affs;
    std::vector<PublicationID> references_of_id = publications_map.at(publicationid).references;

    //Delete mention of publication from all of it's affiliations
    for ( const auto& aff : affs_of_id ) {
        auto& pubs_of_aff = affiliations_map.at(aff).related_pubs;
        pubs_of_aff.erase(std::remove(pubs_of_aff.begin(), pubs_of_aff.end(), publicationid), pubs_of_aff.end());
    }

    //Delete publicationid, if it is a parent to any Publication
    for ( auto& r : references_of_id) {
        publications_map.at(r).parent = NO_PUBLICATION;
    }

    //Delete from pubIDList
    pubIDList.erase(std::remove(pubIDList.begin(), pubIDList.end(), publicationid), pubIDList.end());

    //Delete from publications_map
    auto i2 = publications_map.find(publicationid);
    publications_map.erase(i2);

    return true;
}


