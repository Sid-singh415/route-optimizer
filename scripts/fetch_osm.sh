#!/usr/bin/env bash
# One-time manual fetch of Chandigarh + Mohali road data from the Overpass API.
# Not run at build/runtime -- re-run manually whenever you want fresher OSM data.
set -euo pipefail

cd "$(dirname "$0")/.."

BBOX="30.64,76.70,30.78,76.83" # south,west,north,east -- Chandigarh + Mohali IT City (Amity), with margin
OUT="data/chandigarh_roads.json"

QUERY='[out:json][timeout:60];
(
  way["highway"~"^(motorway|trunk|primary|secondary|tertiary|unclassified|residential|living_street|service|motorway_link|trunk_link|primary_link|secondary_link|tertiary_link)$"]
     ('"$BBOX"');
);
out body;
>;
out skel qt;'

# overpass-api.de frequently returns a busy/406 error page under load;
# the kumi.systems mirror has been more reliable in practice.
OVERPASS_URL="${OVERPASS_URL:-https://overpass.kumi.systems/api/interpreter}"

curl -sS "$OVERPASS_URL" --data-urlencode "data=${QUERY}" -o "$OUT" --max-time 280
echo "Saved $(wc -c < "$OUT" | tr -d ' ') bytes to $OUT"
