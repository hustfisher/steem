#include <steemit/protocol/block.hpp>
#include <fc/io/raw.hpp>
#include <fc/bitutil.hpp>
#include <algorithm>

namespace steemit { namespace protocol {
   digest_type block_header::digest()const
   {
      return digest_type::hash(*this);
   }

   uint32_t block_header::num_from_id(const block_id_type& id)
   {
      return fc::endian_reverse_u32(id._hash[0]);
   }

   /**
    * 计算block id的算法：
    * 1 用本singed block header的内容计算出hash；
    * 2 将该hash的第一个字节设为本block的block num；
    * 3 将hashcpy到block_id_type后返回。
    */
   block_id_type signed_block_header::id()const
   {
      auto tmp = fc::sha224::hash( *this );
      tmp._hash[0] = fc::endian_reverse_u32(block_num()); // store the block num in the ID, 160 bits is plenty for the hash
      static_assert( sizeof(tmp._hash[0]) == 4, "should be 4 bytes" );
      block_id_type result;
      memcpy(result._hash, tmp._hash, std::min(sizeof(result), sizeof(tmp)));
      return result;
   }

   /**
    * 根据witness_signature和digest构建public_key
    */
   fc::ecc::public_key signed_block_header::signee()const
   {
      return fc::ecc::public_key( witness_signature, digest(), true/*enforce canonical*/ );
   }

   /**
    * 根据signer 的private key 构建witness_signature
    */
   void signed_block_header::sign( const fc::ecc::private_key& signer )
   {
      witness_signature = signer.sign_compact( digest() );
   }

   /**
    * signee需要和构建出来的public key相同
    */
   bool signed_block_header::validate_signee( const fc::ecc::public_key& expected_signee )const
   {
      return signee() == expected_signee;
   }

   /**
    * 对signed block中所有的trasaction进行merkle digest，然后计算merkle root值。
    */
   checksum_type signed_block::calculate_merkle_root()const
   {
      if( transactions.size() == 0 )
         return checksum_type();

      vector<digest_type> ids;
      ids.resize( transactions.size() );
      for( uint32_t i = 0; i < transactions.size(); ++i )
         ids[i] = transactions[i].merkle_digest();

      vector<digest_type>::size_type current_number_of_hashes = ids.size();
      while( current_number_of_hashes > 1 )
      {
         // hash ID's in pairs
         uint32_t i_max = current_number_of_hashes - (current_number_of_hashes&1);
         uint32_t k = 0;

         for( uint32_t i = 0; i < i_max; i += 2 )
            ids[k++] = digest_type::hash( std::make_pair( ids[i], ids[i+1] ) );

         if( current_number_of_hashes&1 )
            ids[k++] = ids[i_max];
         current_number_of_hashes = k;
      }
      return checksum_type::hash( ids[0] );
   }

} } // steemit::protocol
