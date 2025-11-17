#pragma once

public interface struct RDLevelListener {
	void TileChanged(int x, int y, int z);

	void LightColumnChanged(int x0, int z0, int x1, int z1);

	void AllChanged();
};