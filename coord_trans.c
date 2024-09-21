// Assessment Coordinate Transformation: 
// Implement coordinate conversion utilities that can convert radar coordinates (bearing, range) 
// to GIS coordinates (lat, lon) and vice versa

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
#define EARTH_RADIUS 6371.0 // Earth's radius in kilometers

// Function prototypes
double deg2rad(double deg);
double rad2deg(double rad);
void RtoG(double range, double bearing, double glonInit, double glatInit, double *glonFinal, double *glatFinal);
void GIS2Radar(double *range, double *bearing, double glonInit, double glatInit, double glonFinal, double glatFinal);
double dist2pt(double lat1, double lon1, double lat2, double lon2);

int main() {
    // Initial location: Wallops Island
    double glatInit = 37.0;
    double glonInit = -75.0;

    // Final location: Puerto Rico
    double glatFinal = 18.0;
    double glonFinal = -66.0;

    // Variables to store the results
    double bearing, range;

    // Convert GIS to radar coordinates
    GIS2Radar(&range, &bearing, glonInit, glatInit, glonFinal, glatFinal);
    printf("Reference location: lat=%f, long=%f\nTarget location: lat=%f, lon=%f:\n", 
      glatInit, glonInit, glatFinal, glonFinal);
    printf("Converted to Radar Coordinates:\n");
    printf("Bearing: %f degrees, Range: %f meters\n\n", bearing, range);

    // Convert back to GIS coordinates
    RtoG(range, bearing, glonInit, glatInit, &glonFinal, &glatFinal);
    printf("Bearing and Range converted back to GIS Coordinates using the same reference location\n");
    printf("Target location: lat=%f, lon=%f\n", glatFinal, glonFinal);

    return 0;
}

// Function implementations (same as provided earlier)
double deg2rad(double deg) {
    return deg * (PI / 180.0);
}

double rad2deg(double rad) {
    return rad * (180.0 / PI);
}

// calculate the great-circle distance between two points on the surface of a sphere
// use the latitudes and longitudes
double dist2pt(double lat1, double lon1, double lat2, double lon2) {
    double dLat = deg2rad(lat2 - lat1);
    double dLon = deg2rad(lon2 - lon1);
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

void RtoG(double range, 
          double bearing,
          double glonInit, 
          double glatInit, 
          double *glonFinal, 
          double *glatFinal) {
    double bearingRad = deg2rad(bearing);
    // Convert range to kilometers
    double rangeKm = range / 1000.0; 

    *glatFinal = asin(sin(deg2rad(glatInit)) * cos(rangeKm / EARTH_RADIUS)
               + cos(deg2rad(glatInit)) * sin(rangeKm / EARTH_RADIUS) * cos(bearingRad));
    *glonFinal = deg2rad(glonInit) + atan2(
                 sin(bearingRad) * sin(rangeKm / EARTH_RADIUS) 
               * cos(deg2rad(glatInit)), cos(rangeKm / EARTH_RADIUS) - sin(deg2rad(glatInit)) * sin(*glatFinal));

    *glatFinal = rad2deg(*glatFinal);
    *glonFinal = rad2deg(*glonFinal);
}

void GIS2Radar(double *range, 
               double *bearing, 
               double glonInit, 
               double glatInit, 
               double glonFinal,
               double glatFinal) {
    double dLon = deg2rad(glonFinal - glonInit);
    double y = sin(dLon) * cos(deg2rad(glatFinal));
    double x = cos(deg2rad(glatInit)) * sin(deg2rad(glatFinal)) -
               sin(deg2rad(glatInit)) * cos(deg2rad(glatFinal)) * cos(dLon);
    *bearing = fmod(rad2deg(atan2(y, x)) + 360.0, 360.0);
    // Convert range to meters
    *range = dist2pt(glatInit, glonInit, glatFinal, glonFinal) * 1000.0; 
}


