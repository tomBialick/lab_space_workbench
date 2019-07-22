#!/bin/bash
# Start the server with this script

# Colors for text
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo ""
echo -e "${GREEN}=======================================================================================${NC}"
echo -e "${GREEN}Starting Server${NC}"
echo -e "${GREEN}=======================================================================================${NC}"
echo ""
npm start


# Keep this uncommented
echo ""
echo -e "${RED}=======================================================================================${NC}"
echo -e "${RED}If you can read this, something probably failed (Unless you manually stopped it)...${NC}"
echo -e "${RED}=======================================================================================${NC}"
echo ""