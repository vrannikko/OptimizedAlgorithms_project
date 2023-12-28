// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_set>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: getting a vector's size is constant time operation
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: only linear operations
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: returning vec involves copying all values -> O(n)
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find + insert = O(n+m)
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: worst case if hash collisions
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: worst case if hash collisions
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort is O(nlogn)
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::sort is O(nlogn)
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: might have to go through map n times
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: might have to go through map n times
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(n+m) because of for loop and std::find
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: getting a vector's size is constant time operation
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating through pubIDList
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating through pubIDList
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is O(n)
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is O(n)
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is O(n)
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find x 2 O(n) + map.at() (O(1)) = O(n)
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is O(n)
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: map.find() is usually constant for unordered_map
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::find and for-loop O(n) + std::sort O(nlogn) = O(nlogn)
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: recursion/complexity depends on depth of chain
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: might have to go through all the involved vectors
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: might have to go through entire affIDList and all publications
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(3n)->O(n), might have to go through std::find once, id1_parents twice
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: might have to go through entire pubIDList, all refs, and all affs
    bool remove_publication(PublicationID publicationid);

    //ID vectors
    std::vector<AffiliationID> affIDList;
    std::vector<PublicationID> pubIDList;

    //Structs
    struct AffiliationData {
        Name name;
        Coord coords;
        std::vector<PublicationID> related_pubs;
    };

    struct PublicationData {
        Name name;
        Year year;
        std::vector<AffiliationID> related_affs;
        std::vector<PublicationID> references;
        PublicationID parent;
    };

    //Maps
    std::unordered_map<AffiliationID, AffiliationData> affiliations_map;
    std::unordered_map<PublicationID, PublicationData> publications_map;
    std::unordered_map<Coord, AffiliationID, CoordHash> coord_to_id_map;

    //Flag variables to avoid running functions unnecessarily
    bool alphabetical = false;
    bool distance_increasing = false;

private:

};

#endif // DATASTRUCTURES_HH
